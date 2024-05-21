#include "syscall.h"
#include "operator.h"
#include "history_buffer.h"
#include "stack.h"

#include <stdio.h>

enum parser_status { ps_ok, ps_eof, ps_illegal_expr };

enum parser_states { pst_read, pst_num_read };

typedef struct parser {
    char ch;
    int number;
    enum parser_states st;
} parser_t;

enum expr_item_types { eit_op, eit_int, eit_var };

typedef struct expr_item {
    enum expr_item_types type;
    union {
        char op;
        int number;
        char var[sizeof(int)];
    };
} expr_item_t;

enum evulator_state { est_init, est_err, est_num, est_op };

const static char evulator_err_msg[] = "Illegal expression item: ";

typedef struct expr_evulator {
    char_stack_t op_st;
    int_stack_t num_st;
    enum evulator_state state; 
} expr_evulator_t;

enum { max_expr_size = 1000 };

typedef struct expression {
    int size;
    expr_item_t items[max_expr_size];
} expression_t;

int is_digit(char ch)
{
    return ch >= '0' && ch <= '9';
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


void print_int(int res)
{
    enum { num_buf_size = 10 };
    char buf[num_buf_size];
    int len;

    len = int_to_str(res, buf, num_buf_size);
    write(1, buf, len);
    write(1, "\n", 1);
}


void append_char_to_number(char ch, int *number)
{
    *number *= 10;
    *number += ch - '0'; 
}

void expression_init(expression_t *expr)
{
    expr->size = 0;
}

void expr_evulator_init(expr_evulator_t *evulator)
{
    char_stack_init(&evulator->op_st);
    int_stack_init(&evulator->num_st);
    evulator->state = est_init;
    char_stack_push('(', &evulator->op_st);
}

void expr_evulator_destroy(expr_evulator_t *evulator)
{
    char_stack_destroy(&evulator->op_st);
    int_stack_destroy(&evulator->num_st);
}

void expression_add_number(int number, expression_t *expr)
{
    expr_item_t *item;
    item = &expr->items[expr->size];
    item->type = eit_int;
    item->number = number;
    expr->size += 1;
}

void expression_add_op(char op, expression_t *expr)
{
    expr_item_t *item;
    item = &expr->items[expr->size];
    item->type = eit_op;
    item->op = op;
    expr->size += 1;
}

void parser_handle_symbol(parser_t *parser, expression_t *expr)
{
    switch (parser->st) {
    case pst_read:
        if (is_operator(parser->ch)) {
            expression_add_op(parser->ch, expr);
            break;
        }
        if (!is_digit(parser->ch))
            break;
        parser->number = 0;
        parser->st = pst_num_read;
        /* go to pst_num_read state handler */

    case pst_num_read:
        if (!is_digit(parser->ch)) {
            expression_add_number(parser->number, expr);
            if (is_operator(parser->ch))
                expression_add_op(parser->ch, expr);
            parser->st = pst_read;
            break;
        }
        
        append_char_to_number(parser->ch, &parser->number);
        break;
        
    default:
        break;
    }
}

enum parser_status parse_expression(expression_t *expr)
{
    parser_t parser;

    parser.st = pst_read;

    while (read(0, &parser.ch, 1) > 0) {
        parser_handle_symbol(&parser, expr);
        if (parser.ch == '\n')
            return ps_ok;
    }

    if (parser.st == pst_num_read)
        expression_add_number(parser.number, expr);

    return ps_eof;
}

/*
void expression_handle_item(const expr_item_t *item, char_stack_t *op_st,
                            int_stack_t *num_st)
{
    } else if (is_operator(parser->ch)) {
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
*/

void evulate_unary_operation(char op, expr_evulator_t *evulator)
{
    int_stack_push(0, &evulator->num_st);
    char_stack_push(op, &evulator->op_st);
    evulator->state = est_op;
}

void evulate_operator(char op, expr_evulator_t *evulator)
{
    while (!char_stack_empty(&evulator->op_st)) {
        char top_op;
        top_op = char_stack_top(&evulator->op_st);
        if (top_op == '(') {
            if (op == ')')
                char_stack_pop(&evulator->op_st);
            else
                char_stack_push(op, &evulator->op_st);
            return;
        } else if (operator_priority(op) <= operator_priority(top_op)) {
            char_stack_pop(&evulator->op_st);
            perform_operation(top_op, &evulator->num_st);
        } else {
            char_stack_push(op, &evulator->op_st);
            return;
        }
    }
}

void evulator_process_lonely_operator(char op, expr_evulator_t *evulator)
{
    if (is_open_bracket(op)) {
        evulate_operator(op, evulator);
        evulator->state = est_op;
    } else if (is_unary_operation(op)) {
        evulate_unary_operation(op, evulator);
        evulator->state = est_op;
    } else {
        evulator->state = est_err;
    }
}

void evulator_handle_init_state(const expr_item_t *item,
                                expr_evulator_t *evulator)
{
    switch (item->type) {
    case eit_op:
        evulator_process_lonely_operator(item->op, evulator);
        break;
    case eit_int:
        int_stack_push(item->number, &evulator->num_st);
        evulator->state = est_num;
        break;
    case eit_var:
        break;
    default:
        break;
    }
}

void evulator_handle_operator_state(const expr_item_t *item,
                                    expr_evulator_t *evulator)
{
    switch (item->type) {
    case eit_op:
        evulator_process_lonely_operator(item->op, evulator);
        break;
    case eit_int:
        int_stack_push(item->number, &evulator->num_st);
        evulator->state = est_num;
        break;
    case eit_var:
        break;
    default:
        break;
    }
}

void evulator_handle_number_state(const expr_item_t *item,
                                  expr_evulator_t *evulator)
{
    switch (item->type) {
    case eit_op:
        evulate_operator(item->op, evulator);
        if (is_close_bracket(item->op)) {
            evulator->state = est_num;
            break;
        }
        evulator->state = est_op;
        break;
    case eit_int:
        int_stack_push(item->number, &evulator->num_st);
        evulator->state = est_num;
        break;
    case eit_var:
        break;
    default:
        break;
    }
}

void evulate_expr_item(const expr_item_t *item, expr_evulator_t *evulator)
{

    switch (evulator->state) {
    case est_init:
        evulator_handle_init_state(item, evulator);
        break;
    case est_op:
        evulator_handle_operator_state(item, evulator);
        break;
    case est_num:
        evulator_handle_number_state(item, evulator);
        break;
    default:
        break;
    }
}

int calc_expression(const expression_t *expr)
{
    expr_evulator_t evulator;
    int i, res;

    res = 0;

    expr_evulator_init(&evulator);

    for (i = 0; i < expr->size; i++) {
        evulate_expr_item(&expr->items[i], &evulator);
        if (evulator.state == est_err) {
            write(1, evulator_err_msg, sizeof(evulator_err_msg)-1);
            goto quit;
        }
    }

    handle_stack_operators(&evulator.op_st, &evulator.num_st);
    res = int_stack_top(&evulator.num_st);

quit:
    expr_evulator_destroy(&evulator);
    return res;
}

/*
void handle_symbol(struct parser_state *parser, char_stack_t *op_st,
                   int_stack_t *num_st)
{
    if (parser->ch >= '0' && parser->ch <= '9') {
        append_char_to_number(parser->ch, &parser->number);
        parser->was_num = 1;
        parser->was_op = 0;
    } else if (is_operator(parser->ch)) {
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
*/

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
    while (status == ps_ok) {
        int i, res;
        expression_init(&expr);
        status = parse_expression(&expr);
        if (expr.size == 0)
            continue;
        res = calc_expression(&expr);
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
