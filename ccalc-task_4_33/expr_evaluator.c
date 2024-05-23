#include "expr_evaluator.h"
#include "operator.h"
#include "syscall.h"
#include "util.h"

char expr_eval_err_msg[max_eval_err_msg_len];

static const char brackets_err_msg[] = "Wrong brackets count!";
static const char illegal_num_seq_err_msg[] = "Illegal numbers sequence: ";
static const char illegal_op_seq_err_msg[] = "Illegal operators sequence: ";

static void make_illegal_num_seq_err_msg(int a, int b)
{
    char buf[sizeof(int)];
    int_to_str(a, buf, sizeof(int));
    str_concat(illegal_num_seq_err_msg, buf, expr_eval_err_msg, 
                     max_eval_err_msg_len);
    str_concat(expr_eval_err_msg, " ", expr_eval_err_msg, 
                     max_eval_err_msg_len);
    int_to_str(b, buf, sizeof(int));
    str_concat(expr_eval_err_msg, buf, expr_eval_err_msg, 
                     max_eval_err_msg_len);
}

static void make_illegal_op_seq_err_msg(char op1, char op2)
{
    enum { buflen = 2 };
    char buf[buflen];
    buf[0] = op1;
    buf[1] = '\0';
    str_concat(illegal_op_seq_err_msg, buf, expr_eval_err_msg, 
               max_eval_err_msg_len);
    buf[0] = op2;
    str_concat(expr_eval_err_msg, buf, expr_eval_err_msg, 
               max_eval_err_msg_len);
}

static void expr_evaluator_init(expr_evaluator_t *eval)
{
    char_stack_init(&eval->op_st);
    int_stack_init(&eval->num_st);
    eval->state = est_init;
    eval->status = es_ok;
    char_stack_push('(', &eval->op_st);
}

static void expr_evaluator_destroy(expr_evaluator_t *eval)
{
    char_stack_destroy(&eval->op_st);
    int_stack_destroy(&eval->num_st);
}

static void evaluate_unary_operation(char op, expr_evaluator_t *eval)
{
    int_stack_push(0, &eval->num_st);
    char_stack_push(op, &eval->op_st);
    eval->state = est_op;
}

static void evaluate_operator(char op, expr_evaluator_t *eval)
{
    while (!char_stack_empty(&eval->op_st)) {
        char top_op;
        top_op = char_stack_top(&eval->op_st);
        if (top_op == '(') {
            if (op == ')')
                char_stack_pop(&eval->op_st);
            else
                char_stack_push(op, &eval->op_st);
            return;
        } else if (operator_priority(op) <= operator_priority(top_op)) {
            char_stack_pop(&eval->op_st);
            perform_operation(top_op, &eval->num_st);
        } else {
            char_stack_push(op, &eval->op_st);
            return;
        }
    }
}

static void eval_process_lonely_operator(char op, 
                                             expr_evaluator_t *eval)
{
    if (is_open_bracket(op)) {
        evaluate_operator(op, eval);
        eval->state = est_num;
    } else if (is_close_bracket(op)) {
        evaluate_operator(op, eval);
        eval->state = est_op;
    } else if (is_unary_operation(op)) {
        evaluate_unary_operation(op, eval);
        eval->state = est_num;
    } else {
        make_illegal_op_seq_err_msg(op, char_stack_top(&eval->op_st));
        eval->status = es_err;
    }
}

static void eval_handle_init_state(const expr_item_t *item,
                                       expr_evaluator_t *eval)
{
    switch (item->type) {
    case eit_op:
        eval_process_lonely_operator(item->op, eval);
        break;
    case eit_int:
        int_stack_push(item->number, &eval->num_st);
        eval->state = est_op;
        break;
    case eit_var:
        break;
    default:
        break;
    }
}

static void eval_handle_operator_state(const expr_item_t *item,
                                           expr_evaluator_t *eval)
{
    switch (item->type) {
    case eit_op:
        evaluate_operator(item->op, eval);
        if (is_close_bracket(item->op)) {
            if (char_stack_empty(&eval->op_st)) {
                eval->status = es_err;
                str_copy(brackets_err_msg, expr_eval_err_msg, 
                         max_eval_err_msg_len);
                break;
            }
            eval->state = est_op;
            break;
        }
        eval->state = est_num;
        break;
    case eit_int:
        make_illegal_num_seq_err_msg(int_stack_top(&eval->num_st), 
                                     item->number);
        eval->status = es_err;
        /*
        int_stack_push(item->number, &eval->num_st);
        eval->state = est_num;
        */
        break;
    case eit_var:
        break;
    default:
        break;
    }
}

static void eval_handle_number_state(const expr_item_t *item,
                                         expr_evaluator_t *eval)
{
    switch (item->type) {
    case eit_op:
        eval_process_lonely_operator(item->op, eval);
        break;
    case eit_int:
        int_stack_push(item->number, &eval->num_st);
        eval->state = est_op;
        break;
    case eit_var:
        break;
    default:
        break;
    }
}

static void evaluate_expr_item(const expr_item_t *item,
                              expr_evaluator_t *eval)
{
    switch (eval->state) {
    case est_init:
        eval_handle_init_state(item, eval);
        break;
    case est_op:
        eval_handle_operator_state(item, eval);
        break;
    case est_num:
        eval_handle_number_state(item, eval);
        break;
    default:
        break;
    }
}

static int check_correct_evaluation(const expr_evaluator_t *eval)
{
    return char_stack_size(&eval->op_st) == 1 && 
                char_stack_top(&eval->op_st) == '(';
}

enum expr_eval_status eval_expression(const expression_t *expr, int *res)
{
    expr_evaluator_t eval;
    enum expr_eval_status status;
    int i;

    *res = 0;

    expr_evaluator_init(&eval);

    for (i = 0; i < expr->size; i++) {
        evaluate_expr_item(&expr->items[i], &eval);
        if (eval.status == es_err)
            goto quit;
    }

    handle_stack_operators(&eval.op_st, &eval.num_st);
    if (!check_correct_evaluation(&eval)) {
        eval.status = es_err;
        str_copy(brackets_err_msg, expr_eval_err_msg, 
                 max_eval_err_msg_len);
        goto quit;
    }
    *res = int_stack_top(&eval.num_st);

quit:
    status = eval.status;
    expr_evaluator_destroy(&eval);
    return status;
}
