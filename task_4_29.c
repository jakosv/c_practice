#include <stdio.h>
#include <stdlib.h>

typedef double (*arr3p)[3];
typedef arr3p (*func_arr3p)(int, arr3p);

arr3p f(int len, arr3p vecp)
{
    int i;
    for (i = 0; i < len; i++) {
        int j;
        int size = sizeof(vecp[i]) / sizeof(double);
        printf("row %d\n", i);
        for (j = 0; j < size; j++) {
            printf("%f ", vecp[i][j]); 
            vecp[i][j] *= i; 
        }
    }
    putchar('\n');
    return vecp;
}


int main()
{
    double arr[][3] = {{1.5, 2.3, 3.14}, {1.5, 2.3, 3.14}, {1.5, 2.3, 3.14}};
    int size = sizeof(arr) / (sizeof(double) * 3);
    int i;
    arr3p p;
    func_arr3p fptr;
    fptr = f;
    p = fptr(size, arr);
    for (i = 0; i < size; i++) {
        int j;
        int size = sizeof(p[i]) / sizeof(double);
        printf("row %d\n", i);
        for (j = 0; j < size; j++) {
            printf("%f ", p[i][j]); 
        }
    }
    putchar('\n');
    return 0;
}
