#include "syscall.h"
#include "operator.h"
#include "history_buffer.h"
#include "stack.h"
#include "util.h"
#include "expr_parser.h"
#include "expr_evulator.h"

int main()
{

    /*
    history_buffer_t history;
    */

    enum parser_status status;

    expression_t expr;

    /*
    history_init(&history);
    */

    status = ps_ok;
    while (status != ps_eof) {
        int res;
        expression_init(&expr);
        status = parse_expression(&expr);
        if (status == ps_err) {
            print_str(parser_err_msg);
            status = ps_ok;
            continue;
        }
        if (expr.size == 0)
            continue;
        res = evulate_expression(&expr);
        print_int(res);
    }

    /*
    status = ps_ok;
    while (status == ps_ok) {
        status = calc_expression(&op_st, &num_st);
        if (!int_stack_empty(&num_st)) {
            int res = int_stack_top(&num_st);
            print_int(res);
            history_add(res, &history);
        }
        char_stack_destroy(&op_st);
        int_stack_destroy(&num_st);
    }
    */

    return 0;
}
