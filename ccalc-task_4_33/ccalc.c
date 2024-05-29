#include "syscall.h"
#include "operator.h"
#include "history_buffer.h"
#include "stack.h"
#include "util.h"
#include "expr_parser.h"
#include "expr_evaluator.h"

int read_line(char *buf, int buflen)
{
    int cnt, res;
    char ch;
    cnt = 0;
    while ((res = read(0, &ch, 1)) > 0) {
        if (ch == '\n' || cnt >= buflen)
            return cnt;
        buf[cnt] = ch;
        cnt += 1;  
    }
    return cnt > 0 ? cnt : -1;
}

void perform_command(char *line_buf, int buflen, history_buffer_t *hist)
{
    if (line_buf[0] == 'q') {
        exit(28);
    } else if (line_buf[0] == 'c') {
        history_clear(hist);
        history_add(0, hist);
    }
}

void print_answer(const history_buffer_t *history)
{
    print_str("$");
    print_int(history_size(history)-1);
    print_str(" = ");
    print_int_line(history_top(history));
}

int main()
{
    enum { buflen = 256 };
    char line_buf[buflen];
    int res;
    history_buffer_t history;
    expression_t expr;

    history_init(&history);
    history_add(0, &history);

    while ((res = read_line(line_buf, buflen)) != -1) {
        enum parser_status status;
        enum expr_eval_status eval_status;
        
        perform_command(line_buf, res, &history);
        expression_init(&expr);
        status = parse_expression(line_buf, res, &expr);
        if (status == ps_err) {
            print_str_line(parser_err_msg);
            continue;
        }
        if (expr.size == 0)
            continue;
        eval_status = eval_expression(&expr, &history);
        if (eval_status == es_err)
            print_str_line(expr_eval_err_msg);
        else
            print_answer(&history);
    }

    history_destroy(&history);

    return 0;
}
