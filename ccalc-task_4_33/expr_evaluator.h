#ifndef EXPR_EVALUATOR_H_SENTRY
#define EXPR_EVALUATOR_H_SENTRY

#include "expression.h"
#include "stack.h"

enum expr_eval_state { est_init, est_num, est_op };
enum expr_eval_status { es_ok, es_err };

enum { max_eval_err_msg_len = 256 };

extern char expr_eval_err_msg[];

typedef struct expr_evaluator {
    char_stack_t op_st;
    int_stack_t num_st;
    enum expr_eval_state state; 
    enum expr_eval_status status; 
} expr_evaluator_t;

enum expr_eval_status eval_expression(const expression_t *expr, int *res);

#endif
