#include "expr_evulator.h"
#include "operator.h"
#include "syscall.h"

static void expr_evulator_init(expr_evulator_t *evulator)
{
    char_stack_init(&evulator->op_st);
    int_stack_init(&evulator->num_st);
    evulator->state = est_init;
    char_stack_push('(', &evulator->op_st);
}

static void expr_evulator_destroy(expr_evulator_t *evulator)
{
    char_stack_destroy(&evulator->op_st);
    int_stack_destroy(&evulator->num_st);
}

static void evulate_unary_operation(char op, expr_evulator_t *evulator)
{
    int_stack_push(0, &evulator->num_st);
    char_stack_push(op, &evulator->op_st);
    evulator->state = est_op;
}

static void evulate_operator(char op, expr_evulator_t *evulator)
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

static void evulator_process_lonely_operator(char op, 
                                             expr_evulator_t *evulator)
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

static void evulator_handle_init_state(const expr_item_t *item,
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

static void evulator_handle_operator_state(const expr_item_t *item,
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

static void evulator_handle_number_state(const expr_item_t *item,
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

static void evulate_expr_item(const expr_item_t *item,
                              expr_evulator_t *evulator)
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

int evulate_expression(const expression_t *expr)
{
    expr_evulator_t evulator;
    int i, res;

    res = 0;

    expr_evulator_init(&evulator);

    for (i = 0; i < expr->size; i++) {
        evulate_expr_item(&expr->items[i], &evulator);
        if (evulator.state == est_err) {
            /*
            write(1, evulator_err_msg, sizeof(evulator_err_msg)-1);
            */
            goto quit;
        }
    }

    handle_stack_operators(&evulator.op_st, &evulator.num_st);
    res = int_stack_top(&evulator.num_st);

quit:
    expr_evulator_destroy(&evulator);
    return res;
}
