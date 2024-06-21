#ifndef GENERIC_STACK_H_SENTRY
#define GENERIC_STACK_H_SENTRY

#include "malloc.h"

#define DECLARE_STACK(TYPE)                                 \
    typedef struct TYPE ## stack_item {                     \
        TYPE data;                                          \
        struct TYPE ## stack_item_t *next;                  \
    } TYPE ## _stack_item_t;                                \
                                                            \
    typedef struct TYPE ## _stack {                         \
        int size;                                           \
        TYPE ## _stack_item_t *head;                        \
    } TYPE ## _stack_t;                                     \
                                                            \
    void TYPE ## _stack_init(TYPE ## _stack_t *st);         \
    int TYPE ## _stack_push(TYPE op, TYPE ## _stack_t *st); \
    TYPE TYPE ## _stack_top(const TYPE ## _stack_t *st);    \
    int TYPE ## _stack_pop(TYPE ## _stack_t *st);           \
    int TYPE ## _stack_empty(const TYPE ## _stack_t *st);   \
    void TYPE ## _stack_destroy(TYPE ## _stack_t *st);      \
    int TYPE ## _stack_size(const TYPE ## _stack_t *st);    \

#define DEFINE_STACK(TYPE)                                  \
    void TYPE ## _stack_init(TYPE ## _stack_t *st)          \
    {                                                       \
        st->head = NULL;                                    \
        st->size = 0;                                       \
    }                                                       \
                                                            \
    int TYPE ## _stack_push(TYPE n, TYPE ## _stack_t *st)   \
    {                                                       \
        TYPE ## _stack_item_t *tmp;                         \
        tmp = malloc(sizeof(*tmp));                         \
        tmp->data = n;                                      \
        tmp->next = st->head;                               \
        st->head = tmp;                                     \
        st->size += 1;                                      \
        return 0;                                           \
    }                                                       \
                                                            \
    TYPE TYPE ## _stack_top(const TYPE ## _stack_t *st)     \
    {                                                       \
        if (!st->head)                                      \
            return 0;                                       \
        return st->head->data;                              \
    }                                                       \
                                                            \
    int TYPE ## _stack_pop(TYPE ## _stack_t *st)            \
    {                                                       \
        TYPE ## _stack_item_t *tmp;                         \
        if (!st->head)                                      \
            return 1;                                       \
        tmp = st->head;                                     \
        st->head = st->head->next;                          \
        free(tmp);                                          \
        st->size -= 1;                                      \
        return 0;                                           \
    }                                                       \
                                                            \
    int TYPE ## _stack_empty(const TYPE ## _stack_t *st)    \
    {                                                       \
        return st->size == 0;                               \
    }                                                       \
                                                            \
    void TYPE ## _stack_destroy(TYPE ## _stack_t *st)       \
    {                                                       \
        TYPE ## _stack_item_t *tmp;                         \
        while (st->head) {                                  \
            tmp = st->head;                                 \
            st->head = st->head->next;                      \
            free(tmp);                                      \
        }                                                   \
        st->size = 0;                                       \
    }                                                       \
                                                            \
    int TYPE ## _stack_size(const TYPE ## _stack_t *st)     \
    {                                                       \
        return st->size;                                    \
    }                                                       \

#endif 
