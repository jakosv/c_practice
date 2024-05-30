#ifndef SYSCALL_H_SENTRY
#define SYSCALL_H_SENTRY

extern int errno;

int read(int fd, char *buf, int count);
int write(int fd, const char *buf, int count);
void exit(int status);

#endif
