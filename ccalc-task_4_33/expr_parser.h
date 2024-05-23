#ifndef EXPR_PARSER_H_SENTRY
#define EXPR_PARSER_H_SENTRY

#include "expression.h"
#include "stack.h"

extern char parser_err_msg[];

enum parser_status { ps_ok, ps_eoln, ps_eof, ps_err };

enum parser_status parse_expression(expression_t *expr);

#endif
