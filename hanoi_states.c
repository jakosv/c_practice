#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum tstate { state_new, state_wait, state_done };
struct task {
    int amount, from, to;
    enum tstate state;
    struct task *next;
};

struct task *create_task(int amount, int from, int to, enum tstate state,
                                                        struct task *next)
{
    struct task *new_task;
    new_task = malloc(sizeof(*new_task));
    new_task->amount = amount;
    new_task->from = from;
    new_task->to = to;
    new_task->state = state;
    new_task->next = next;
    return new_task;
}

int main(int argc, char **argv)
{
    struct task *first;
    int amount, from, to;

    if(argc < 4) {
        fprintf(stderr, "Too few arguments: hanoi_states [n] [from] [to]\n");
        return 1;
    }
    amount = atoi(argv[1]);
    if(amount < 0) {
        fprintf(stderr, "Invalid number of rings: %d\n", amount);
        return 2;
    }
    from = atoi(argv[2]);
    if(from < 1 || from > 3) {
        fprintf(stderr, "Invalid start bar number: %d\n", from); 
        return 3;
    }
    to = atoi(argv[3]);
    if(to < 1 || to > 3) {
        fprintf(stderr, "Invalid end bar number: %d\n", to); 
        return 4;
    }
    if (from == to)
        return 0;

    first = create_task(amount, from, to, state_new, NULL);

    while(first) {
        struct task *tmp;

        switch(first->state) {
        case state_new:
            first->state = state_wait;
            if(first->amount > 1) {
                tmp = create_task(first->amount - 1, first->from, 
                    6 - (first->to + first->from), state_new, first);
                first = tmp;
            }
            break;
        case state_wait:
            first->state = state_done;
            printf("%d: %d -> %d\n", first->amount, first->from, first->to);
            if(first->amount > 1) {
                tmp = create_task(first->amount - 1, 
                    6 - (first->to + first->from), first->to, state_new, first);
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
