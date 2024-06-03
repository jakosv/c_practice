#ifndef OPEN_H_SENTRY
#define OPEN_H_SENTRY

enum {
    O_RDONLY    = 0x0000,
    O_WRONLY    = 0x0001,
    O_RDWR      = 0x0002,
    O_CREAT     = 0x0200,
    O_EXCL      = 0x0800,
    O_TRUNC     = 0x0400,
    O_APPEND    = 0x0008
};

#endif
