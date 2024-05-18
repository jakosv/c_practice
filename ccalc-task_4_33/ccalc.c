#include "syscall.h"
#include "operator.h"
#include "history_buffer.h"
#include "stack.h"

enum parser_status { ps_ok, ps_eof, ps_illegal_expr };

struct parser_state {
    char ch;
    int number;
    int was_num, was_op;
    enum parser_status status;
};

void append_char_to_number(char ch, int *number)
{
    *number *= 10;
    *number += ch - '0'; 
}

void handle_unary_operation_symbol(struct parser_state *parser,
                                   char_stack_t *op_st, int_stack_t *num_st)
{
    parser->number = 0;
    int_stack_push(parser->number, num_st);
    char_stack_push(parser->ch, op_st);
}

void handle_symbol(struct parser_state *parser, char_stack_t *op_st,
                   int_stack_t *num_st)
{
    if (parser->ch >= '0' && parser->ch <= '9') {
        append_char_to_number(parser->ch, &parser->number);
        parser->was_num = 1;
        parser->was_op = 0;
    } else if (operator_priority(parser->ch) > 0) {
        if (!parser->was_num && is_unary_operation(parser->ch)) {
            handle_unary_operation_symbol(parser, op_st, num_st);
            return;
        } 
        if (parser->was_num) {
            int_stack_push(parser->number, num_st);
            parser->was_num = 0;
            parser->number = 0;
        }
        sift_operator(parser->ch, op_st, num_st);

        parser->was_op = 1;
    }
}

enum parser_status calc_expression(char_stack_t *op_st, int_stack_t *num_st)
{
    struct parser_state parser;

    char_stack_init(op_st);
    int_stack_init(num_st);

    char_stack_push('(', op_st);

    parser.number = 0;
    parser.was_num = 0;
    parser.status = ps_eof;

    while (read(0, &parser.ch, 1) > 0) {
        if (parser.ch == '\n') {
            parser.status = ps_ok;
            break;
        }
        handle_symbol(&parser, op_st, num_st);
    }

    if (parser.was_num)
        int_stack_push(parser.number, num_st);

    handle_stack_operators(op_st, num_st);

    return parser.status;
}

void str_reverse(char *str, int len)
{
    int i;
    for (i = 0; i < len / 2; i++) {
        char a, b;
        a = str[i];
        b = str[len - i - 1];

        a ^= b;
        b = a ^ b;
        a ^= b;

        str[i] = a;
        str[len - i - 1] = b;
    }
}

int int_to_str(int n, char *buf, int size)
{
    int i, sign;

    sign = n < 0;
    if (sign) {
        n *= -1;
        size -= 1;
        *buf = '-';
        buf++;
    }

    i = 0;
    while (n > 0) {
        char ch = n % 10 + '0';
        buf[i] = ch;
        i++;
        if (i >= size - 1)
            break;
        n /= 10;
    }
    str_reverse(buf, i);
    buf[i] = '\0';

    return i + sign;
}

void print_result(int res)
{
    enum { num_buf_size = 10 };
    char buf[num_buf_size];
    int len;

    len = int_to_str(res, buf, num_buf_size);
    write(1, buf, len);
    write(1, "\n", 1);
}

int main()
{
    char_stack_t op_st;
    int_stack_t num_st;


    /*
    history_buffer_t history;
    */

    enum parser_status status;

    /*
    history_init(&history);
    */

    status = ps_ok;
    while (status == ps_ok) {
        status = calc_expression(&op_st, &num_st);
        if (!int_stack_empty(&num_st)) {
            int res = int_stack_top(&num_st);
            print_result(res);
            /*
            history_add(res, &history);
            */
        }
        char_stack_destroy(&op_st);
        int_stack_destroy(&num_st);
    }

    return 0;
}
