#ifndef NUM_STACK_H_SENTRY
#define NUM_STACK_H_SENTRY

struct num_stack_item {
    int number;
    struct num_stack_item *next;
};

typedef struct num_stack {
    int size;
    struct num_stack_item *top;
} num_stack_t;

void num_stack_init(num_stack_t *st);
void num_stack_push(int num, num_stack_t *st);
int num_stack_top(const num_stack_t *st);
void num_stack_pop(num_stack_t *st);
void num_stack_perform_op(char op, num_stack_t *st);
int num_stack_empty(const num_stack_t *st);
void num_stack_destroy(num_stack_t *st);

#endif
