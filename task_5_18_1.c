#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int fd, pid;

    if (argc < 2) {
        fprintf(stderr, "Too few arguments!\n");
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        dup2(fd, 0);
        execvp(argv[2], argv + 2);
    }

    wait(NULL);
    close(fd);

    return 0;
}
