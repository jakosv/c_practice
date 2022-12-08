#include <stdio.h>
#include <stdlib.h>

enum { max_length = 50 };

int string_length(const char *s)
{
    const char *p;
    for (p = s; *p; p++)
        {}
    return p - s;
}

double string_to_double(const char *s)
{
    double res, frac_part;
    int ceil_part, base, sign, i, slen;

    sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    base = 10;
    ceil_part = 0;
    for (; *s && *s != '.'; s++)
        ceil_part = ceil_part * base + (*s - '0');

    if (!(*s))
        return (double)sign * (double)ceil_part;

    s++;
    frac_part = 0;
    slen = string_length(s);
    for (i = slen - 1; i >= 0; i--) {
        frac_part += (double)(s[i] - '0');
        frac_part /= (double)base;
    }
    res = (double)ceil_part + frac_part;
    res *= (double)sign;
    return res;
}

void double_to_string(double num, int precision, char *s)
{
    int ceil, last_not_zero, base, i, slen;
    char buf[max_length];

    slen = 0;
    if (num < 0) {
        s[slen] = '-';
        slen++;
        num *= -1.0;
    }

    base = 10;
    ceil = (int)num;
    if (!ceil) {
        s[slen] = '0';
        slen++;
    }
    num -= (double)ceil;
    for (i = 0; ceil > 0; i++, ceil /= 10)
        buf[i] = (ceil % base + '0');
    for (i--; i >= 0; i--, slen++)
        s[slen] = buf[i]; 

    last_not_zero = -1;
    for (i = 0; precision > 0 && i < max_length; precision--, i++) {
        num *= (double)base;
        ceil = (int)num;
        num -= (double)ceil;
        buf[i] = ceil + '0';  
        if (buf[i] != '0')
            last_not_zero = i;
    } 
    if (last_not_zero != -1) {
        s[slen] = '.';
        slen++;
    }
    for (i = 0; i <= last_not_zero && slen < max_length; i++, slen++)
        s[slen] = buf[i];
    s[slen] = 0;
}

int main(int argc, char **argv)
{
    double x, y, multi;
    int precision;
    char str[max_length];
    if (argc < 4) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }
    precision  = atoi(argv[3]);
    x = string_to_double(argv[1]);
    y = string_to_double(argv[2]);
    multi = x * y;
    double_to_string(x, precision, str);
    printf("x = %s\n", str);
    double_to_string(y, precision, str);
    printf("y = %s\n", str);
    double_to_string(multi, precision, str);
    printf("x * y = %s\n", str);
    return 0;
}
