#include "op_stack.h"

#include <stdlib.h>

void op_stack_init(op_stack_t *st)
{
    st->size = 0;
    st->top = NULL;
}

void op_stack_push(char op, op_stack_t *st)
{
    struct op_stack_item *tmp;
    tmp = malloc(sizeof(*tmp));
    tmp->oper = op;
    tmp->next = st->top;;
    st->top = tmp;
    st->size += 1;
}

char op_stack_top(const op_stack_t *st)
{
    return st->top->oper;
}

void op_stack_pop(op_stack_t *st)
{
    struct op_stack_item *tmp;
    tmp = st->top;
    st->top = st->top->next;
    st->size -= 1;
    free(tmp);
}

int op_stack_empty(const op_stack_t *st)
{
    return st->size <= 0;
}

void op_stack_destroy(op_stack_t *st)
{
    struct op_stack_item *tmp;
    while (st->top) {
        tmp = st->top;
        st->top = st->top->next;
        free(tmp);
    }
    st->size = 0;
}

int op_stack_size(const op_stack_t *st)
{
    return st->size;
}

