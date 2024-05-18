#ifndef OP_STACK_H_SENTRY
#define OP_STACK_H_SENTRY

enum { max_op_stack_size = 1000 };

typedef struct op_stack {
    int size;
    int top_idx;
    char data[max_op_stack_size];
} op_stack_t;

void op_stack_init(op_stack_t *st);
void op_stack_push(char op, op_stack_t *st);
char op_stack_top(const op_stack_t *st);
void op_stack_pop(op_stack_t *st);
int op_stack_empty(const op_stack_t *st);
void op_stack_destroy(op_stack_t *st);
int op_stack_size(const op_stack_t *st);

#endif
