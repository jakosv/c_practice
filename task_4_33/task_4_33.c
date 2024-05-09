#include "op_stack.h"
#include "num_stack.h"
#include "operator.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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
                                   op_stack_t *op_st, num_stack_t *num_st)
{
    parser->number = 0;
    num_stack_push(parser->number, num_st);
    op_stack_push(parser->ch, op_st);
}


void handle_symbol(struct parser_state *parser, op_stack_t *op_st,
                   num_stack_t *num_st)
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
            num_stack_push(parser->number, num_st);
            parser->was_num = 0;
            parser->number = 0;
        }
        sift_operator(parser->ch, op_st, num_st);

        parser->was_op = 1;
    }
}

enum parser_status calc_expression(op_stack_t *op_st, num_stack_t *num_st)
{
    struct parser_state parser;

    op_stack_init(op_st);
    num_stack_init(num_st);

    op_stack_push('(', op_st);
    /*
    num_stack_push(0, num_st);
    */

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
        num_stack_push(parser.number, num_st);

    handle_stack_operators(op_st, num_st);


    return parser.status;
}

int main()
{
    op_stack_t op_st;
    num_stack_t num_st;
    enum parser_status status;

    status = ps_ok;
    while (status == ps_ok) {
        status = calc_expression(&op_st, &num_st);
        /*
        printf("ans: %d\n", num_stack_top(&num_st));
        */
        if (!num_stack_empty(&num_st)) {
            printf("%d\n", num_stack_top(&num_st));
        }
        op_stack_destroy(&op_st);
        num_stack_destroy(&num_st);
    }

    /*
    while (!op_stack_empty(&op_st)) {
        char op = op_stack_top(&op_st);
        op_stack_pop(&op_st);
        printf("%c ", op);
    }

    while (!num_stack_empty(&num_st)) {
        int num = num_stack_top(&num_st);
        num_stack_pop(&num_st);
        printf("%d ", num);
    }

    putchar('\n');
    */

    return 0;
}
