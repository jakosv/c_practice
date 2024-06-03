#include "syscall.h"
#include "open.h"
#include "util.h"

int main(int argc, char **argv)
{
    enum { buflen = 4096 };
    char buf[buflen];
    int src, dst, res;

    if (argc < 3) {
        print_str_line("Too few arguments.");
        return 1;
    }

    src = open(argv[1], O_RDONLY);
    if (src == -1) {
        print_str(argv[1]);
        print_str_line(": open for reading error.");
        return 2;
    }

    dst = open(argv[2], O_WRONLY|O_CREAT, 0666);
    if (dst == -1) {
        print_str(argv[2]);
        print_str_line(": open for writing error.");
        return 3;
    }

    while ((res = read(src, buf, buflen)) > 0)
        write(dst, buf, res);
    
    return 0;
}
