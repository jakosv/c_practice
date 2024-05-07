#include <stdlib.h>
#include <stdio.h>

#define ABS(A) ((A)>=0 ? (A) : -(A))

struct item {
    int n;
    struct item *next;
};

void list_add(int n, struct item **lst)
{
    struct item *tmp;

    tmp = *lst;
    if (tmp && ABS(n - tmp->n) >= 5)
        return;

    tmp = malloc(sizeof(struct item));
    tmp->next = *lst;
    tmp->n = n;
    *lst = tmp;
}

int main()
{
    int n, res;
    struct item *head;
    struct item *node;

    head = NULL;

    while ((res = scanf("%d", &n)) != EOF)
        list_add(n, &head);

    for (node = head; node && node->next; node = node->next)
        printf("%d %d\n", node->n, node->next->n);
    
    return 0;
}
