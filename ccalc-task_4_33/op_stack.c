#include "op_stack.h"

#include <stdlib.h>

void op_stack_init(op_stack_t *st)
{
    st->size = 0;
}

void op_stack_push(char op, op_stack_t *st)
{
    if (st->size >= max_op_stack_size)
        return;
    st->data[st->size] = op;
    st->size += 1;
}

char op_stack_top(const op_stack_t *st)
{
    return st->data[st->size-1];
}

void op_stack_pop(op_stack_t *st)
{
    if (st->size == 0)
        return;
    st->size -= 1;
}

int op_stack_empty(const op_stack_t *st)
{
    return st->size == 0;
}

void op_stack_destroy(op_stack_t *st)
{
    st->size = 0;
}

int op_stack_size(const op_stack_t *st)
{
    return st->size;
}

