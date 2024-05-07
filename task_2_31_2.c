#include <stdio.h>
#include <stdlib.h>

typedef double (*fptr)(double);
typedef fptr (*repfptr)(fptr);


double g(double n)
{
    printf("g: %f\n", n);
    return n;
}

double fi(double n)
{
    printf("fi: %f\n", n);
    return n;
}

fptr save = g;

fptr f(fptr new_f)
{
    fptr old;
    old = save;
    save(228);
    save = new_f;
    return old;
}

int main()
{
    fptr old;
    repfptr p = f;
    save(228);
    old = p(fi);
    old(228);
    save(228);
    return 0;
}
