#ifndef EXPR_EVULATOR_H_SENTRY
#define EXPR_EVULATOR_H_SENTRY

#include "expression.h"
#include "stack.h"

enum evulator_state { est_init, est_err, est_num, est_op };

enum { max_evulator_err_msg_len = 256 };

static const char expr_evulator_error[max_evulator_err_msg_len];

typedef struct expr_evulator {
    char_stack_t op_st;
    int_stack_t num_st;
    enum evulator_state state; 
} expr_evulator_t;

int evulate_expression(const expression_t *expr);

#endif
