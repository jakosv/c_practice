#ifndef MMAP_H_SENTRY
#define MMAP_H_SENTRY

#define PAGE_SIZE       4096

#define PROT_NONE       0x00
#define PROT_READ       0x01
#define PROT_WRITE      0x02
#define PROT_EXEC       0x04

#define MAP_FAILED      ((void *)-1)

#define MAP_SHARED      0x0001
#define MAP_PRIVATE     0x0002

#define MAP_FIXED       0x10


#ifdef OS_LINUX

#define MAP_ANONYMOUS   0x20 
#define MAP_ANON        MAP_ANONYMOUS

#else

#define MAP_ANON        0x1000
#define MAP_ANONYMOUS   MAP_ANON

#endif


#endif
