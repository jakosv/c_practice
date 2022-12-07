#include <stdio.h>
#include <stdlib.h>

enum { max_length = 50 };

double string_to_double(const char *s)
{
    double res, point_base;
    int ceil_part, frac_part, base, sign;

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

    frac_part = 0;
    point_base = 1;
    for (s++; *s; s++) {
        frac_part = frac_part * base + (*s - '0');
        point_base /= 10;
    }
    res = (double)ceil_part + point_base * (double)frac_part;
    res *= (double)sign;
    return res;
}

void double_to_string(double num, int precision, char *s)
{
    int ceil, last_digit, base, i;
    char buf[max_length];
    if (num < 0) {
        *s = '-';
        s++;
        num *= -1.0;
    }
    base = 10;
    ceil = (int)num;
    if (!ceil) {
        *s = '0';
        s++;
    }
    num -= (double)ceil;
    for (i = 0; ceil > 0; i++, ceil /= 10)
        buf[i] = (ceil % base + '0');
    for (i--; i >= 0; i--, s++)
        *s = buf[i]; 
    last_digit = -1;
    for (i = 0; precision > 0; i++, precision--) {
        num *= (double)base;
        ceil = (int)num;
        num -= ceil;
        buf[i] = ceil + '0';  
        if (buf[i] != '0')
            last_digit = i;
    } 
    if (last_digit != -1) {
        *s = '.';
        s++;
    }
    for (i = 0; i <= last_digit; i++, s++)
        *s = buf[i];
    *s = 0;
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
