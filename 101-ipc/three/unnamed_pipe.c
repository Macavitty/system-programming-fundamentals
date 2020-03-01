#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER 4096

int write_to_pipe_from_file(int fd, int pipe_fd) {
    char buff_orig[BUFFER];
    char buff_even[BUFFER / 2];
    int bytes;
    while ((bytes = read(fd, buff_orig, BUFFER)) > 0) {
        int j = 0;
        for (int i = 0; i < bytes; i += 2, j++) {
            buff_even[j] = buff_orig[i];
        }
        if (write(pipe_fd, buff_even, j) == -1){
            return -1;
        }
    }
    if (bytes < 0)
        return -1;
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s path_to_file\n", argv[0]);
    }

    int pipe_fd[2]; // [0] - read, [1] - write

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    switch (fork()) {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
        case 0: // child
            printf("child\n");
            if (close(pipe_fd[1]) < 0)
                perror("close pipe`s write end from child");
            // read
            dup2(pipe_fd[0], STDIN_FILENO);
            execlp("wc", "wc", "-c", (char *) NULL);

            if (close(pipe_fd[0]) < 0)
                perror("close pipe`s read end from child");
            return EXIT_SUCCESS;
        default: // parent
        {
            printf("parent\n");
            int fd;
            if (close(pipe_fd[0]) < 0) {
                perror("close pipe`s read end from parent");
            }
            if ((fd = open(argv[1], O_RDONLY)) < 0) {
                perror("open");
                return EXIT_FAILURE;
            }
            if (write_to_pipe_from_file(fd, pipe_fd[1]) < 0){
                perror("write_to_pipe_from_file");
                return EXIT_FAILURE;
            }
            if (close(fd) < 0) {
                perror("close");
            }
            if (close(pipe_fd[1]) < 0)
                perror("close pipe`s write end from parent");
            wait(NULL);
            return EXIT_SUCCESS;
        }
    }
}