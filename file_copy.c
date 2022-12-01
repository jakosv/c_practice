#include <stdio.h>
#include <stdlib.h>

void file_copy(FILE *src, FILE *dst)
{
    enum { bsize = sizeof(char), bcount = 4096 }; 
    int n;
    char *bptr = malloc(bcount * bsize);
    while (!feof(src)) {
        n = fread(bptr, bsize, bcount, src);
        if (ferror(src)) {
            fprintf(stderr, "fcopy(): something went wrong while reading\n");
            exit(1);
        } 
        fwrite(bptr, bsize, n, dst);
        if (ferror(src)) {
            fprintf(stderr, "fcopy(): something went wrong while writing\n");
            exit(1);
        } 
    }
    free(bptr);
}

int main(int argc, char **argv)
{
    FILE *src, *dst;
    const char *src_mode = "r";
    const char *dst_mode = "w";
    if (argc < 3) {
        fprintf(stderr, "To few arguments\n");
        return 1;
    }
    src = fopen(argv[1], src_mode);
    if (!src) {
        perror(argv[1]);    
        return 2;
    } 
    dst = fopen(argv[2], dst_mode);
    if (!dst) {
        perror(argv[2]);
        return 3;
    }
    file_copy(src, dst);
    fclose(src);
    fclose(dst);
    return 0;
}
