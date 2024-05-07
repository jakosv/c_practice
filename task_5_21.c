#include <unistd.h>
#include <signal.h>
#include <errno.h>

const char msg[] = "Ctrl-C count: ";

volatile sig_atomic_t ctrl_c_cnt;

void str_reverse(char str[], int len)
{
    int i = 0;
    for (i = 0; i < len / 2; i++) {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
}

int num_to_str(int n, char num_buf[], int buflen)
{
    int idx = 0;
    while (idx < buflen-1) {
        char c = (n % 10) + '0';
        num_buf[idx] = c;
        idx += 1;
        n /= 10; 
        if (n == 0)
            break;
    }
    str_reverse(num_buf, idx);
    num_buf[idx] = '\0';
    return idx;
}

void ctrl_c_handler(int s)
{
    signal(SIGINT, ctrl_c_handler);
    ctrl_c_cnt += 1;
}

void usr1_handler(int s)
{
    enum { buflen = 19 };
    char num_buf[buflen];
    int save_errno, len;

    signal(SIGUSR1, usr1_handler);

    save_errno = errno;

    write(1, msg, sizeof(msg)-1);
    len = num_to_str(ctrl_c_cnt, num_buf, buflen);
    write(1, num_buf, len);
    write(1, "\n", 1);

    errno = save_errno;
}

int main()
{
    ctrl_c_cnt = 0;

    signal(SIGUSR1, usr1_handler);
    signal(SIGINT, ctrl_c_handler);

    while (1)
        pause();

    return 0;
}
