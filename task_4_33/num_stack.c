#include "num_stack.h"

#include <stdlib.h>

void num_stack_init(num_stack_t *st)
{
    st->size = 0;
    st->top = NULL;
}

void num_stack_push(int num, num_stack_t *st)
{
    struct num_stack_item *tmp;
    tmp = malloc(sizeof(*tmp));
    tmp->number = num;
    tmp->next = st->top;;
    st->top = tmp;
    st->size += 1;
}

int num_stack_top(const num_stack_t *st)
{
    return st->top->number;
}

void num_stack_pop(num_stack_t *st)
{
    struct num_stack_item *tmp;

    tmp = st->top;
    st->top = st->top->next;
    st->size -= 1;
    free(tmp);
}

int num_stack_empty(const num_stack_t *st)
{
    return st->size <= 0;
}

void num_stack_destroy(num_stack_t *st)
{
    struct num_stack_item *tmp;
    while (st->top) {
        tmp = st->top;
        st->top = st->top->next;
        free(tmp);
    }
    st->size = 0;
}
