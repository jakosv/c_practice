#ifndef VARIABLE_H_SENTRY
#define VARIABLE_H_SENTRY

#include "expression.h"
#include "history_buffer.h"

enum { var_char = '$' };

enum history_err replace_expr_variables(const history_buffer_t *hist,
                                        expression_t *expr);

#endif
