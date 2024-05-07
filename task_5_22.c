#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

char c;
volatile sig_atomic_t alarm_int;

void write_char()
{
    write(1, &c, 1);
}

void int_handler(int s)
{
    signal(SIGINT, int_handler);

    if (alarm_int)
        exit(1);

    alarm_int = 1;

    c = '+';
}

void quit_handler(int s)
{
    signal(SIGQUIT, quit_handler);
    c = '-';
}

void alarm_handler(int s)
{
    signal(SIGALRM, alarm_handler);
    alarm_int = 0;
    alarm(1);
    write_char();
}

int main()
{
    signal(SIGINT, int_handler);
    signal(SIGQUIT, quit_handler);
    signal(SIGALRM, alarm_handler);

    c = '+';

    alarm_int = 0;
    alarm(1);

    while (1)
        pause();

    return 0;
}
