#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum tstate { state_new, state_wait, state_done };
struct task {
    int amount, src, dst;
    enum tstate state;
    struct task *next;
};

struct task *create_task(int amount, int src, int dst, enum tstate state,
                                                        struct task *next)
{
    struct task *new_task;
    new_task = malloc(sizeof(*new_task));
    new_task->amount = amount;
    new_task->src = src;
    new_task->dst = dst;
    new_task->state = state;
    new_task->next = next;
    return new_task;
}

int interm_rod(int src, int dst)
{
    return 1 + 2 + 3 - dst - src;
}

int main(int argc, char **argv)
{
    struct task *first;
    int amount, src, dst;

    if(argc < 4) {
        fprintf(stderr, "Too few arguments: [amount] [from] [to]\n");
        return 1;
    }
    amount = atoi(argv[1]);
    if(amount < 0) {
        fprintf(stderr, "Invalid number of rings: %d\n", amount);
        return 2;
    }
    src = atoi(argv[2]);
    if(src < 1 || src > 3) {
        fprintf(stderr, "Invalid start rod number: %d\n", src); 
        return 3;
    }
    dst = atoi(argv[3]);
    if(dst < 1 || dst > 3) {
        fprintf(stderr, "Invalid end rod number: %d\n", dst); 
        return 4;
    }
    if (src == dst)
        return 0;

    first = create_task(amount, src, dst, state_new, NULL);

    while(first) {
        struct task *tmp;

        switch(first->state) {
        case state_new:
            first->state = state_wait;
            if(first->amount > 1) {
                tmp = create_task(first->amount - 1, first->src, 
                    interm_rod(first->src, first->dst), state_new, first);
                first = tmp;
            }
            break;
        case state_wait:
            first->state = state_done;
            printf("%d: %d -> %d\n", first->amount, first->src, first->dst);
            if(first->amount > 1) {
                tmp = create_task(first->amount - 1, 
                    interm_rod(first->src, first->dst), first->dst, state_new, first);
                first = tmp;
            }
            break;
        case state_done:
            tmp = first;
            first = tmp->next;
            free(tmp);
        }
    } 
    return 0;
}
