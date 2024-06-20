#include "syscall.h"
#include "operator.h"
#include "history_buffer.h"
#include "stack.h"
#include "util.h"
#include "expr_parser.h"
#include "expr_evaluator.h"
#include "variable.h"
#include "mmap.h"
#include "malloc.h"

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

int perform_command(char *line_buf, int buflen, history_buffer_t *hist)
{
    if (line_buf[0] == 'q') {
        exit(0);
    } else if (line_buf[0] == 'c') {
        print_str_line("History has been cleared");
        history_clear(hist);
        history_add(0, hist);
        return 1;
    }
    return 0;
}

void print_answer(int res, int pos)
{
    print_str("$");
    print_int(pos);
    print_str(" = ");
    print_int_line(res);
}

int main()
{
    enum { buflen = 256 };
    char line_buf[buflen];
    int res;
    int *test;
    history_buffer_t history;
    expression_t expr;

    /*
    test = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE,
                MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);

    if (test == MAP_FAILED) {
        print_str_line("MAP FAILED");
        return 1;
    }
    *test = 228;
    print_int_line(*test);
    test = munmap(test, PAGE_SIZE);
    *test = 228;
    if (test == MAP_FAILED) {
        print_str_line("UNMAP FAILED");
        return 1;
    }
    */
    test = malloc(sizeof(int));
    *test = 228;
    print_int_line(*test);
    free(test);
    *test = 228;

    history_init(&history);
    history_add(0, &history);

    while ((res = read_line(line_buf, buflen)) != -1) {
        enum parser_status parse_status;
        enum history_err hist_err;
        enum expr_eval_status eval_status;
        int cmd_res;
        
        cmd_res = perform_command(line_buf, res, &history);
        if (cmd_res > 0)
            goto next_line;

        expression_init(&expr);
        parse_status = parse_expression(line_buf, res, &expr);
        if (parse_status == ps_err) {
            print_str_line(parser_err_msg);
            goto next_line;
        }
        if (expr.size == 0)
            goto next_line;

        hist_err = replace_expr_variables(&history, &expr);
        if (hist_err == hs_out_of_range) {
            print_str_line("Variable number out of range");
            goto next_line;
        }

        eval_status = eval_expression(&expr, &res);
        if (eval_status == es_err) {
            print_str_line(expr_eval_err_msg);
        } else {
            hist_err = history_add(res, &history);
            if (hist_err == hs_full) {
                print_int_line(res);
                print_str_line("History is full: use 'c' command to clear");
            } else {
                print_answer(res, history_size(&history)-1);
            }
        }
next_line:
        expression_destroy(&expr);
    }

    history_destroy(&history);

    return 0;
}
