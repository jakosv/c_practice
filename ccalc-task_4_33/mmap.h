#ifndef MMAP_H_SENTRY
#define MMAP_H_SENTRY

enum { MAP_FAILED = -1 };

enum { 
    PROT_READ = 1, 
    PROT_WRITE = 2, 
    PROT_NONE = 8, 
    PROT_EXEC = 4
};

enum { 
    MAP_SHARED = 4, 
    MAP_PRIVATE = 2, 
    MAP_FIXED = 1, 
    MAP_ANONYMOUS = 4096
};

#endif
