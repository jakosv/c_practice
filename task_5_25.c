#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

const char msg[] = "Hello, There!\n General Kenoby!\n";

int main()
{
    enum { buflen = 1024 };
    char buf[buflen];
    int pid, res;
    int fd[2];

    pipe(fd);
    printf("in: %d, out: %d\n", fd[0], fd[1]);

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        write(fd[1], msg, sizeof(msg)-1);
        _exit(0); 
    }

    close(fd[1]);

    while ((res = read(fd[0], buf, buflen)) > 0)
        write(1, buf, res);

    close(fd[0]);

    wait(NULL);

    return 0;
}
