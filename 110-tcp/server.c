#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <zconf.h>
#include <netinet/in.h>
#include <dirent.h>
#include <signal.h>

#define MAX_PORT 65535
#define QUEUE_LENGTH 12
static const char * EOT = "\0";
static const char *USAGE = "./server port";
static const char *PORT_RANGE = "port must be an integer in range [1; 65535]";

static int sock_fd;
static bool is_running;
static int parent_pid;

void interrupt(int sig) {
    is_running = false;
    close(sock_fd);
    if (getpid() == parent_pid)
        printf("\n* Got %s signal\n* Closing connections...\n", strsignal(sig));
}


int send_dir_content(int fd, const char *dir_path) {
    DIR *dir;
    if ((dir = opendir(dir_path)) == NULL) {
        char *buf = (char *) malloc(2048);
        sprintf(buf, "Could not open directory \"%s\". Error %s\n\n", dir_path, strerror(errno));
        write(fd, buf, strlen(buf));
        free(buf);
        return -1;
    }

    char *buf = (char *) malloc(2048);
    sprintf(buf, "Entry of \"%s\":\n", dir_path);
    write(fd, buf, strlen(buf));
    free(buf);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *tmpbuf = (char *) malloc(2048);
        sprintf(tmpbuf, "\t%s\n", entry->d_name);
        write(fd, tmpbuf, strlen(tmpbuf));
        free(tmpbuf);
    }
    char end[] = "\n";
    write(fd, end, strlen(end));
    closedir(dir);
    return 0;
}

int handle_client(int client) {
    int len = 0;
    char *buf = (char *) malloc(2048 * sizeof(char));
    int bytes_read;
    char *request = NULL;
    while ((bytes_read = read(client, buf, 2048)) > 0) {
        request = realloc(request, len + bytes_read);
        memcpy(request + len, buf, bytes_read);
        len += bytes_read;
        if (*(request + len - 1) == EOT[0]) {
//            request[len - 1] = '\0';
            break;
        }
    }
    printf("* Requested:\n%s", request);

    printf("* Sending answer ...\n");
    char *dirs = strtok(request, "\n");
    while (dirs != NULL) {
        send_dir_content(client, dirs);
        dirs = strtok(NULL, "\n");
    }
    write(client, EOT, 1);
    printf("* Done\n");
    free(request);
    close(client);
    return 0;
}

int run(unsigned int port) {
    // CREATE socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("get sock_fd");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(addr.sin_zero), 8);

    if (bind(sock_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(sock_fd, QUEUE_LENGTH) < 0) {
        perror("listen");
        return -1;
    }
    fprintf(stdout, "* Listening on %s:%d\n", "localhost", port);

    // main loop
    is_running = true;
    int client;
    while (is_running && (client = accept(sock_fd, NULL, NULL)) > 0) {
        if (!fork()) {
            printf("* Received connection  from client %d\n", client);
            // create child process
            handle_client(client);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // signals handler
    signal(SIGINT, interrupt);
    signal(SIGHUP, interrupt);
    signal(SIGTERM, interrupt);

    // set parent PID
    parent_pid = getpid();

    // check args
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\n", USAGE);
        return EXIT_FAILURE;
    }

    // get port
    unsigned int port = strtol(argv[1],  NULL, 10);
    if (port > MAX_PORT) {
        fprintf(stderr, "Error: %s\n", PORT_RANGE);
        return EXIT_FAILURE;
    }

    if (run(port) < 0) {
        return errno;
    }
    return EXIT_SUCCESS;
}