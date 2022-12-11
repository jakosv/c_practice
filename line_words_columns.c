#include <stdio.h>
#include <stdlib.h>

struct str_item {
    char data; 
    int n;
    struct str_item *next;
};

struct lst_item {
    struct str_item *str;
    struct lst_item *next;
};

void string_reverse(struct str_item **str)
{
    struct str_item *prev, *next, *tmp;
    prev = NULL;
    for (tmp = *str; tmp; tmp = next) {
        next = tmp->next;
        tmp->next = prev;
        prev = tmp;
    }
    *str = prev;
}

void string_add(char c, int n, struct str_item **str)
{
    struct str_item *tmp;
    tmp = malloc(sizeof(struct str_item));
    tmp->data = c;
    tmp->n = n;
    tmp->next = *str;
    *str = tmp;
}

void _string_print(struct str_item *str, int *n)
{
    if (!str)
        return;
    _string_print(str->next, n);
    for (; *n < str->n; (*n)++)
        putchar(' ');
    putchar(str->data);
    (*n)++;
}

void string_print(struct str_item *str)
{
    if (!str)
        return;
    int n = 1;
    _string_print(str, &n);
}

void string_destroy(struct str_item *str)
{
    if (!str)
        return;
    string_destroy(str->next);
    free(str);
}

void list_add_string(struct str_item *str, struct lst_item **lst)
{
    if (!str)
        return;
    if (!(*lst)) {
        *lst = malloc(sizeof(struct lst_item));
        (*lst)->str = NULL;
        (*lst)->next = NULL;
    }
    string_add(str->data, str->n, &(*lst)->str);
    list_add_string(str->next, &(*lst)->next); 
}

void list_print(struct lst_item *lst)
{
    if (!lst)
        return;
    string_print(lst->str);
    printf("\n");
    list_print(lst->next);
}

void list_destroy(struct lst_item *lst)
{
    if (!lst)
        return;
    list_destroy(lst->next);
    string_destroy(lst->str);
    free(lst);
}

int main()
{
    struct lst_item *lst = NULL;
    struct str_item *str = NULL;
    int c, str_cnt;
    char last;
    str_cnt = 0;
    last = ' ';
    while ((c = getchar()) != EOF) {
        if ((c == ' ' && last != ' ') || (c == '\n')) {
            str_cnt++;
            string_reverse(&str);
            list_add_string(str, &lst);
            string_destroy(str);
            str = NULL;
            last = ' ';
            if (c == '\n') {
                list_print(lst);
                list_destroy(lst);
                lst = NULL;
                str_cnt = 0;
            }
        } else {
            string_add(c, str_cnt + 1, &str);
            last = c;
        }
    }
    return 0;
}
