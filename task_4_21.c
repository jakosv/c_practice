#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    double *data;
    int size, capacity;
} stackdbl_t;

void stackdbl_init(stackdbl_t *stack)
{
    stack->size = 0;
    stack->capacity = 1;
    stack->data = malloc(stack->capacity * sizeof(double));
}

void stackdbl_destroy(stackdbl_t *stack)
{
    stack->size = 0;
    stack->capacity = 0;
    free(stack->data);
}

void stackdbl_push(double n, stackdbl_t *stack)
{
    if (stack->size >= stack->capacity) {
        double *new_data;
        int new_capacity;

        new_capacity = stack->capacity * 2;
        new_data = malloc(new_capacity * sizeof(double));
        memcpy(new_data, stack->data, stack->capacity * sizeof(double)); 
        stack->capacity = new_capacity;
        free(stack->data);
        stack->data = new_data;
    }
    stack->data[stack->size] = n;
    stack->size += 1;
}

void stackdbl_pop(stackdbl_t *stack, double *n)
{
    stack->size -= 1;
    *n = stack->data[stack->size];
}

int stackdbl_empty(stackdbl_t *stack)
{
    return stack->size <= 0;
}

int main()
{
    enum { size = 9 };
    int i;
    stackdbl_t *stack;

    stack = malloc(sizeof(stackdbl_t));
    stackdbl_init(stack);

    for (i = 0; i < size; i++)
        stackdbl_push(i, stack);

    while (!stackdbl_empty(stack)) {
        double n;
        stackdbl_pop(stack, &n);
        printf("%f\n", n); 
    }

    stackdbl_destroy(stack);
    free(stack);

    return 0;
}
