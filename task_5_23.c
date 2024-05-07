#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

const char wake_up[] = "Is anybody here?\n";

volatile sig_atomic_t lines;
volatile sig_atomic_t chars;
volatile sig_atomic_t show_info;
volatile sig_atomic_t alarm_int;

void int_handler(int s)
{
    signal(SIGINT, int_handler);
    if (alarm_int)
        exit(1);
    show_info = 1;
    alarm_int = 1;
    alarm(5);
}

void alarm_handler(int s)
{
    signal(SIGALRM, alarm_handler);
    write(1, wake_up, sizeof(wake_up)-1);
    alarm_int = 0;
}

int main()
{

    lines = 0;
    chars = 0; 
    show_info = 0;
    alarm_int = 0;

    signal(SIGALRM, alarm_handler);
    signal(SIGINT, int_handler);

    alarm(5);

    do {
        int res;
        char c;

        if (show_info) {
            printf("lines: %d, chars: %d\n", lines, chars);    
            show_info = 0;
        }

        res = read(1, &c, 1);
        if (res == -1 && errno == EINTR)
            continue;

        if (res <= 0)
            break;

        if (c == '\n')
            lines += 1;
        else
            chars += 1;
        alarm(5);
    } while (1);

    return 0;
}
