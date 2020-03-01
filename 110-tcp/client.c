#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define MAX_PORT 65535
static const char * EOT = "\0";
static const char *USAGE = "./client host port dir_1 dir_2 ... dir_n";
static const char *PORT_RANGE = "port must be an integer in range [1; 65535]";

int main(int argc, char *argv[]){
    // check args number
    if (argc < 3) {
        fprintf(stderr, "Usage: %s\n", USAGE);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "* Started client\n");

    // get host
    char * host_in = argv[1];
    printf("** Got host: %s\n", host_in);

    // get port
    unsigned int port = strtol(argv[2], (char **)NULL, 10);
    if (port > MAX_PORT){
        fprintf(stderr, "Error: %s\n", PORT_RANGE);
        return EXIT_FAILURE;
    }
    if (port == 0){
        if (strncmp(argv[2], "0", strlen(argv[2])) == 0){
            fprintf(stderr, "Error: port number 0 is reserved in TCP\n");
        }
        else{
            fprintf(stderr, "Error: could not parse second argument: %s\n", PORT_RANGE);
        }
        return EXIT_FAILURE;
    }
    printf("** Got port: %d\n", port);

    // get dirs
    int dirs_cnt = argc - 3;
    const char *dirs[dirs_cnt];
    for (int i = 3; i < argc; i++){
        dirs[i - 3] = argv[i];
        printf("** Got dir: %s\n", dirs[i-3]);
    }

    // CREATE socket
    int sock_fd;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("get sock_fd");
        return errno;
    }

    struct sockaddr_in socket;
    struct hostent *host = gethostbyname(host_in);
    // INIT server info
    bzero(&socket, sizeof(socket));
    socket.sin_family = AF_INET;
    socket.sin_port = htons(port); // converts to network byte order
    socket.sin_addr = *((struct in_addr *)host->h_addr); //
    bzero(&(socket.sin_zero), 8);     /* zero the rest of the struct */

    // CONNECT to server
    if (connect(sock_fd, (struct sockaddr*)&socket, sizeof(socket)) != 0 ){
        perror("connect ");
        return errno;
    }
    fprintf(stdout, "* Successfully connected to %s on port %d\n", host_in, port);
    fprintf(stdout, "* Sending request ...\n");

    // send'n'receive
    for (int i = 0; i < dirs_cnt; i++){
        printf("Sending %s\n", dirs[i]);
        write(sock_fd, dirs[i], strlen(dirs[i]));
        write(sock_fd, "\n", 1);
    }
    write(sock_fd, EOT, 1);
    fprintf(stdout, "* Done\n");
    fprintf(stdout, "* Waiting for answer ...\n");
    char *buf = (char*)malloc(2048 * sizeof(char));
    int len = 0;
    int bytes_read;
    char *response = NULL;
    while ((bytes_read = read(sock_fd, buf, 2048)) > 0) {
        response = realloc(response, len + bytes_read);
        memcpy(response + len, buf, bytes_read);
        len += bytes_read;
        if (*(response + len - 1) == EOT[0]){
//            response[len - 1] = '\0';
            break;
        }
    }
    printf("Response:\n%s\n", response);

    // CLOSE
    close(sock_fd);
    return EXIT_SUCCESS;
}
