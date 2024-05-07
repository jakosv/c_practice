#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
    enum { max_num = 1000000, buflen = 8 };
    char buf[buflen];
    int fd[2];
    int pid, i, status;

    if (argc < 2) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    pipe(fd);

    pid = fork();
    if (pid == -1) {

    } else if (pid == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        exit(0);
    }

    close(fd[0]);
    signal(SIGPIPE, SIG_IGN);
    for (i = 1; i <= max_num; i++) {
        int n = sprintf(buf, "%d", i); 
        int res = write(fd[1], buf, n);
        if (res == -1 && errno == EPIPE) {
            perror("PIPE write");
            break;
        }
        usleep(1000);
    }
    close(fd[1]);

    wait(&status);
    
    if (WIFEXITED(status)) {
        printf("exit %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("killed %d\n", WTERMSIG(status)); 
    }

    return 0;
}
