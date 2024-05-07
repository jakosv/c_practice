#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int run_first_program(char **argv, int fd[2])
{
    int p1 = fork();
    if (p1 == -1) {
        perror("fork");
        exit(1);
    } else if (p1 == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execvp(*argv, argv);
        perror(*argv);
        exit(1);
    }

    return p1;
}

int run_second_program(char **argv, int fd[2])
{
    int p2 = fork();
    if (p2 == -1) {
        perror("fork");
        exit(1);
    } else if (p2 == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);

        signal(SIGINT, SIG_IGN);

        execvp(argv[0], argv);
        perror(argv[0]);
        exit(1);
    }

    return p2;
}

void wait_program()
{
    int status;
    int p1 = wait(&status);
    if (WIFEXITED(status)) {
        printf("[%d] exit %d\n", p1, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("[%d] killed %d\n", p1, WTERMSIG(status));
    }
}

int main(int argc, char **argv)
{
    int p1, p2;
    char **args;
    int fd[2];

    if (argc < 3) {
        fprintf(stderr, "Too few arguments!\n");
        exit(1);
    }

    args = argv + 1;
    while (*args && !((*args)[0] == ';' && (*args)[1] == ';'))
        args++;

    if (!args[0] || !args[1]) {
        fprintf(stderr, "Passed only one program!\n");
        exit(1);
    }

    *args = NULL;

    pipe(fd);
    p1 = run_first_program(argv + 1, fd);
    p2 = run_second_program(args + 1, fd);

    close(fd[0]);
    close(fd[1]);

    wait_program();
    wait_program();

    return 0;
}
