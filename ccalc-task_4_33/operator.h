#ifndef OPERATOR_H_SENTRY
#define OPERATOR_H_SENTRY

int perform_binary_operation(int a, int b, char op);
int perform_unary_operation(int a, char op);
int operator_priority(char op);
int is_operator(char op);
int is_unary_operator(char op);
int is_open_bracket(char op);
int is_close_bracket(char op);
int is_bracket(char op);

#endif
