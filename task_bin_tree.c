#include <stdlib.h>
#include <stdio.h>

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

struct node {
    double n;
    struct node *left;
    struct node *right;
};

void tree_init(struct node **root)
{
    *root = NULL;
}

void tree_destroy(struct node *root)
{
    if (!root)
        return;
    tree_destroy(root->left);
    tree_destroy(root->right);
    free(root);
}

struct node **search_node(double n, struct node **root)
{
    if (!*root || (*root)->n == n) {
        printf("%f\n", n);
        return root;
    }

    if ((*root)->n > n) {
        printf(" -l-> ");
        return search_node(n, &((*root)->left));
    }
    printf(" -r-> ");
    return search_node(n, &((*root)->right));
}

void tree_add(double n, struct node **root)
{
    struct node **new_node;
    new_node = search_node(n, root);
    *new_node = malloc(sizeof(struct node));
    (*new_node)->n = n;
    (*new_node)->left = NULL;
    (*new_node)->right = NULL;
}

struct node *tree_search(double n, struct node *root)
{
    if (!root)
        return NULL;

    if (root->n > n)
        return tree_search(n, root->left);
    else
    if (root->n < n)
        return tree_search(n, root->right);

    return root;
}

void tree_traverse(struct node *root, void (*f)(double, void*),
                   void *user_data)
{
    if (!root)
        return;

    tree_traverse(root->left, f, user_data);
    f(root->n, user_data);
    tree_traverse(root->right, f, user_data);
}

struct tree_minmax {
    int cnt;
    double min;
    double max; 
};

void find_minmax(double n, void *user_data)
{
    struct tree_minmax *data = user_data;

    if (data->cnt == 0) {
        data->cnt = 1;
        data->min = n;
        data->max = n;
        return;
    }
    data->min = MIN(data->min, n);
    data->max = MAX(data->max, n);
    data->cnt += 1;
}

void tree_sum(double n, void *userdata)
{
    *(double*)userdata += n;
}

int main()
{
    double n, sum;
    struct node *tree;
    struct tree_minmax data;

    tree_init(&tree);
    while ((scanf("%lf", &n)) != EOF) {
        tree_add(n, &tree);
    }

    data.cnt = 0;
    tree_traverse(tree, find_minmax, &data);
    printf("node count: %d\n", data.cnt);
    printf("min: %f\n", data.min);
    printf("max: %f\n", data.max);

    sum = 0;
    tree_traverse(tree, tree_sum, &sum);
    printf("sum: %f\n", sum);

    tree_destroy(tree);

    return 0;
}
