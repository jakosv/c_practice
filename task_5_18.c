#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd, pid;

    if (argc < 3) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror(argv[1]);
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        dup2(fd, 1);
        execvp(argv[2], argv + 2);
        perror("execvp");
        fflush(stderr);
        _exit(1);
    }

    close(fd);

    return 0;
}
