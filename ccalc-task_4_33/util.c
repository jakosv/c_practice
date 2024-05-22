#include "util.h"
#include "syscall.h"

int is_digit(char ch)
{
    return ch >= '0' && ch <= '9';
}

void str_reverse(char *str, int len)
{
    int i;
    for (i = 0; i < len / 2; i++) {
        char a, b;
        a = str[i];
        b = str[len - i - 1];

        a ^= b;
        b = a ^ b;
        a ^= b;

        str[i] = a;
        str[len - i - 1] = b;
    }
}

int int_to_str(int n, char *buf, int size)
{
    int i, sign;

    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }

    sign = n < 0;
    if (sign) {
        n *= -1;
        size -= 1;
        *buf = '-';
        buf++;
    }

    i = 0;
    while (n > 0) {
        char ch = n % 10 + '0';
        buf[i] = ch;
        i++;
        if (i >= size - 1)
            break;
        n /= 10;
    }
    str_reverse(buf, i);
    buf[i] = '\0';

    return i + sign;
}

void print_int(int res)
{
    enum { num_buf_size = 10 };
    char buf[num_buf_size];
    int len;

    len = int_to_str(res, buf, num_buf_size);
    write(1, buf, len);
    write(1, "\n", 1);
}

void append_char_to_number(char ch, int *number)
{
    *number *= 10;
    *number += ch - '0'; 
}

int str_copy(const char *s, char *d, int max_size)
{
    int i;
    for (i = 0; i < max_size-1 && s[i]; ++i)
        d[i] = s[i]; 
    d[i] = '\0';

    return i;
}

int str_length(const char *s)
{
    const char *p = s;
    for (; *p; p++)
        {};

    return p - s;
}

int str_concat(const char *s1, const char *s2, char *buf, int buflen)
{
    int len;

    for (len = 0; len < buflen-1 && *s1; ++len, ++s1)
        buf[len] = *s1;

    for (; len < buflen-1 && *s2; ++len, ++s2)
        buf[len] = *s2;

    buf[len] = '\0';

    return len;
}

void print_str(const char *s)
{
    int len = str_length(s);
    write(1, s, len); 
    write(1, "\n", 1); 
}
