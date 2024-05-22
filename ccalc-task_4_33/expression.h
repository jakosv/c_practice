#ifndef EXPRESSION_H_SENTRY
#define EXPRESSION_H_SENTRY

enum expr_item_types { eit_op, eit_int, eit_var };

enum { max_expr_size = 1000 };

typedef struct expr_item {
    enum expr_item_types type;
    union {
        char op;
        int number;
        char var[sizeof(int)];
    };
} expr_item_t;

typedef struct expression {
    int size;
    expr_item_t items[max_expr_size];
} expression_t;


void expression_init(expression_t *expr);
void expression_add_number(int number, expression_t *expr);
void expression_add_op(char op, expression_t *expr);

#endif
