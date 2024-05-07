#include <stdio.h>
#include <stdlib.h>

typedef void (*fpar)(double*);
typedef int (*fptr)(fpar);
typedef fptr (*pvec)[4];

int main()
{
    pvec fvecops; 
    return 0;
}
