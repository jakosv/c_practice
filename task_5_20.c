#include <unistd.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t bye;

void handler(int s)
{
    bye = 1; 
}

int main()
{
    printf("Press Ctrl-C to quit\n");

    signal(SIGINT, handler);

    while (!bye)
        pause();

    puts("Good bye");
    return 0;
}
