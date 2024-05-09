#ifndef OPERATOR_H_SENTRY
#define OPERATOR_H_SENTRY

#include "op_stack.h"
#include "num_stack.h"

int perform_binary_operation(int a, int b, char op);
int is_unary_operation(char op);
int perform_unary_operation(int a, char op);
int operator_priority(char op);
void perform_operation(char op, num_stack_t *st);
void handle_stack_operators(op_stack_t *op_st, num_stack_t *num_st);
void sift_operator(char op, op_stack_t *op_st, num_stack_t *num_st);

#endif
