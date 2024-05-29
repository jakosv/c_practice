#include "variable.h"
#include "history_buffer.h"

static enum history_err replace_var_with_value(const history_buffer_t *hist,
                                               expr_item_t *item)
{
    enum history_err res;
    int var_pos;
    var_pos = item->var_num;
    res = history_get(var_pos, hist, &item->number);
    if (res != hs_ok)
        return res;
    item->type = eit_int;
    return hs_ok;
}

enum history_err replace_expr_variables(const history_buffer_t *hist,
                                        expression_t *expr)
{
    int i;
    for (i = 0; i < expr->size; i++) {
        if (expr->items[i].type == eit_var) {
            enum history_err status;
            status = replace_var_with_value(hist, &expr->items[i]);
            if (status != hs_ok)
                return status;
        }
    }
    return hs_ok;
}
