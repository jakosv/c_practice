#ifndef SYSCALL_H_SENTRY
#define SYSCALL_H_SENTRY

typedef unsigned int size_t;
typedef unsigned int off_t;

extern int errno;

int read(int fd, char *buf, int count);
int write(int fd, const char *buf, int count);
void exit(int status);
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t len);

#endif
