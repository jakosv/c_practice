#ifndef EXPR_EVULATOR_H_SENTRY
#define EXPR_EVULATOR_H_SENTRY

#include "expression.h"
#include "stack.h"

enum evulator_state { est_init, est_err, est_num, est_op };
enum expr_evulator_status { es_ok, es_err };

enum { max_evulator_err_msg_len = 256 };

extern char expr_evulator_err_msg[];

typedef struct expr_evulator {
    char_stack_t op_st;
    int_stack_t num_st;
    enum evulator_state state; 
} expr_evulator_t;

enum expr_evulator_status evulate_expression(const expression_t *expr,
                                             int *res);

#endif
