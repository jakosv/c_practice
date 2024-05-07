#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct op_stack_item {
    char op;
    struct op_stack_item *next;
};

typedef struct op_stack {
    int size;
    struct op_stack_item *top;
} op_stack_t;

void op_stack_init(op_stack_t *st)
{
    st->size = 0;
    st->top = NULL;
}

void op_stack_push(char op, op_stack_t *st)
{
    struct op_stack_item *tmp;
    tmp = malloc(sizeof(*tmp));
    tmp->op = op;
    tmp->next = st->top;;
    st->top = tmp;
    st->size += 1;
}

char op_stack_pop(op_stack_t *st)
{
    struct op_stack_item *tmp;
    char res_op;

    res_op = st->top->op;
    tmp = st->top;
    st->top = st->top->next;
    st->size -= 1;
    free(tmp);

    return res_op;
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


int bin_pow(int a, int p)
{
    if (p == 1)
        return a;
    if (p % 2 == 0) {
        int res = bin_pow(a, p / 2);
        return res * res;
    }
    return bin_pow(a, p - 1);
}

int perform_binary_operation(int a, int b, char op)
{
    switch(op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    case '%':
        return a % b;
    case '^':
        return bin_pow(a, b);
    }
    return 0;
}

int perform_unary_operation(int a, char op)
{
    switch(op) {
    case '-':
        return -a;
    case '+':
        return a;
    }
    return 0;
}


struct num_stack_item {
    int num;
    struct num_stack_item *next;
};

typedef struct num_stack {
    int size;
    struct num_stack_item *top;
} num_stack_t;

void num_stack_init(num_stack_t *st)
{
    st->size = 0;
    st->top = NULL;
}

void num_stack_push(int num, num_stack_t *st)
{
    struct num_stack_item *tmp;
    tmp = malloc(sizeof(*tmp));
    tmp->num = num;
    tmp->next = st->top;;
    st->top = tmp;
    st->size += 1;
}

char num_stack_pop(num_stack_t *st)
{
    struct num_stack_item *tmp;
    char res_num;

    res_num = st->top->num;
    tmp = st->top;
    st->top = st->top->next;
    st->size -= 1;
    free(tmp);

    return res_num;
}

void num_stack_perform_op(char op, num_stack_t *st)
{
    int a, b;
    if (st->size < 2) {
        printf("num_stack_perform_op(): too few operands on stack.");
        return;
    }
    a = num_stack_pop(st);
    b = num_stack_pop(st);
    num_stack_push(perform_binary_operation(a, b, op), st);
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

void RPN_algorithm()
{

}

int operator_priority(char op)
{
    switch (op) {
    case '(':
        return 1;

    case '+':
    case '-':
        return 2;

    case '*':
    case '/':
    case '%':
        return 3;

    case '^':
        return 4;

    case ')':
        return 5;
    }

    return 0;
}

int main()
{
    op_stack_t op_st;
    num_stack_t num_st;
    char ch;
    int num, is_unary, was_num;

    op_stack_init(&op_st);
    num_stack_init(&num_st);

    op_stack_push(')', &op_st);

    num = 0;
    was_num = 0;
    is_unary = 0;
    while (read(1, &ch, 1) > 0) {
        if (ch >= '0' && ch <= '9') {
            num *= 10;
            num += ch - '0'; 
            was_num = 1;
        } else if (operator_priority(ch) > 0) {
            if (!was_num) {
                is_unary = 1;
                continue;
            }
            if (is_unary) {
                char unary_op = op_stack_pop(&op_st);
                num = perform_unary_operation(num, unary_op);
                is_unary = 0;
            }
            num_stack_push(num, &num_st);
            op_stack_push(ch, &op_st);
            num = 0;
            was_num = 0;
        }
    }

    if (is_unary) {
        char unary_op = op_stack_pop(&op_st);
        num = perform_unary_operation(num, unary_op);
        is_unary = 0;
    }
    num_stack_push(num, &num_st);


    while (!op_stack_empty(&op_st)) {
        char op = op_stack_pop(&op_st);
        printf("%c ", op);
    }

    while (!num_stack_empty(&num_st)) {
        int num = num_stack_pop(&num_st);
        printf("%d ", num);
    }

    putchar('\n');

    op_stack_destroy(&op_st);
    num_stack_destroy(&num_st);

    return 0;
}
