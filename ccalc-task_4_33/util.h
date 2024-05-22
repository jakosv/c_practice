#ifndef UTIL_H_SENTRY
#define UTIL_H_SENTRY

int is_digit(char ch);
void str_reverse(char *str, int len);
int int_to_str(int n, char *buf, int size);
void print_int(int res);
void append_char_to_number(char ch, int *number);
int str_copy(const char *s, char *d, int max_size);
int str_length(const char *s);
int str_concat(const char *s1, const char *s2, char *buf, int buflen);
void print_str(const char *s);

#endif
