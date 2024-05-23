#ifndef EXPR_EVALUATOR_H_SENTRY
#define EXPR_EVALUATOR_H_SENTRY

#include "expression.h"
#include "history_buffer.h"

enum expr_eval_status { es_ok, es_err };
extern char expr_eval_err_msg[];

enum expr_eval_status eval_expression(const expression_t *expr,
                                      history_buffer_t *results);

#endif
