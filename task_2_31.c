#include <stdio.h>
#include <stdlib.h>

typedef int *arr[10];
typedef arr *parr;
/*
typedef int *(*parr)[10];
*/

parr f(int size)
{
    int arr_len, i, j;
    parr res;

    res = malloc(size * sizeof(arr));
    arr_len = sizeof(arr) / sizeof(int*);

    for (i = 0; i < size; i++) {
        printf("%ld\n", res + i);
        for (j = 0; j < arr_len; j++)
            res[i][j] = (int*)((i+1) * (j+1));
    }
    return res;
}

int main()
{
    enum { size = 3 };
    int arr_len, i, j;
    parr p;

    p = f(size);
    arr_len = sizeof(arr) / sizeof(int*);

    for (i = 0; i < size; i++) {
        for (j = 0; j < arr_len; j++)
            printf("%ld ", (long)p[i][j]);
        putchar('\n');
    }
    return 0;
}
