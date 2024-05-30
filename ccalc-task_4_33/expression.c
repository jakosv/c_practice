#include "expression.h"

void expression_init(expression_t *expr)
{
    expr->size = 0;
}

void expression_destroy(expression_t *expr)
{
    expr->size = 0;
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

void expression_add_variable(int var_num, expression_t *expr)
{
    expr_item_t *item;
    item = &expr->items[expr->size];
    item->type = eit_var;
    item->var_num = var_num;
    expr->size += 1;
}
