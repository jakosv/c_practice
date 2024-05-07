#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>

const char msg[] = "Ctrl-C count: ";

volatile sig_atomic_t ctrl_c_cnt;
volatile sig_atomic_t can_print;

void ctrl_c_handler(int s)
{
    signal(SIGINT, ctrl_c_handler);
    ctrl_c_cnt += 1;
}

void usr1_handler(int s)
{
    signal(SIGUSR1, usr1_handler);
    can_print = 1;
}

int main()
{
    ctrl_c_cnt = 0;
    can_print = 0;

    signal(SIGUSR1, usr1_handler);
    signal(SIGINT, ctrl_c_handler);

    while (1) {
        pause();
        if (can_print) {
            printf("Count: %d\n", ctrl_c_cnt);
            can_print = 0;
        }
    }

    return 0;
}
