#include <stdlib.h>
#include <stdio.h>

struct stack {
    double *data;
    unsigned size, capacity;
};

void stackdbl_init(struct stack *st)
{
    st->data = NULL; 
    st->size = 0;
    st->capacity = 0;
}

void stackdbl_destroy(struct stack *st)
{
    if (st->capacity)
        free(st->data);
    st->data = NULL;
    st->size = 0;
    st->size = 0;
}

void stackdbl_push(double x, struct stack *st)
{
    if (!st->capacity)
        st->capacity = 1;
    if (st->size + 1 == st->capacity) {
        double *new_data;
        unsigned i;
        st->capacity *= 2;
        new_data = malloc(st->capacity * sizeof(double));
        for (i = 0; i < st->size; i++)
            new_data[i] = st->data[i];
        free(st->data);
        st->data = new_data;
    }
    st->data[st->size] = x;
    st->size++;
}

void stackdbl_pop(double *x, struct stack *st)
{
    if (stackdbl_empty(st)) {
        x = NULL;
        return;
    }
    st->size--;
    *x = st->data[st->size];
}

int stackdbl_empty(const struct stack *st)
{
    return st->size == 0;
}

int main()
{
    struct stack st;
    double x;
    stackdbl_init(&st);
    printf("size = %d, capacity = %d\n", st.size, st.capacity);
    stackdbl_push(3.14, &st);
    printf("size = %d, capacity = %d\n", st.size, st.capacity);
    stackdbl_push(123.456, &st);
    printf("size = %d, capacity = %d\n", st.size, st.capacity);
    stackdbl_pop(&x, &st);
    printf("pop: %f\n", x);
    printf("size = %d, capacity = %d\n", st.size, st.capacity);
    stackdbl_pop(&x, &st);
    printf("pop: %f\n", x);
    printf("size = %d, capacity = %d\n", st.size, st.capacity);
    if (stackdbl_empty(&st)) {
        puts("now stack is empty");
    }
    stackdbl_destroy(&st);
    printf("size = %d, capacity = %d\n", st.size, st.capacity);

    return 0;
}
