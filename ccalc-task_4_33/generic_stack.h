#ifndef GENERIC_STACK_H_SENTRY
#define GENERIC_STACK_H_SENTRY

#define DECLARE_STACK(TYPE, MAX_SIZE)                       \
    typedef struct TYPE ## _stack {                         \
        int size;                                           \
        TYPE data[MAX_SIZE];                                \
    } TYPE ## _stack_t;                                     \
                                                            \
    void TYPE ## _stack_init(TYPE ## _stack_t *st);         \
    int TYPE ## _stack_push(TYPE op, TYPE ## _stack_t *st); \
    TYPE TYPE ## _stack_top(const TYPE ## _stack_t *st);    \
    int TYPE ## _stack_pop(TYPE ## _stack_t *st);           \
    int TYPE ## _stack_empty(const TYPE ## _stack_t *st);   \
    void TYPE ## _stack_destroy(TYPE ## _stack_t *st);      \
    int TYPE ## _stack_size(const TYPE ## _stack_t *st);    \

#define DEFINE_STACK(TYPE, MAX_SIZE)                        \
    void TYPE ## _stack_init(TYPE ## _stack_t *st)          \
    {                                                       \
        st->size = 0;                                       \
    }                                                       \
                                                            \
    int TYPE ## _stack_push(TYPE op, TYPE ## _stack_t *st)  \
    {                                                       \
        if (st->size >= MAX_SIZE)                           \
            return 1;                                       \
        st->data[st->size] = op;                            \
        st->size += 1;                                      \
        return 0;                                           \
    }                                                       \
                                                            \
    TYPE TYPE ## _stack_top(const TYPE ## _stack_t *st)     \
    {                                                       \
        return st->data[st->size-1];                        \
    }                                                       \
                                                            \
    int TYPE ## _stack_pop(TYPE ## _stack_t *st)            \
    {                                                       \
        if (st->size == 0)                                  \
            return 1;                                       \
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
        st->size = 0;                                       \
    }                                                       \
                                                            \
    int TYPE ## _stack_size(const TYPE ## _stack_t *st)     \
    {                                                       \
        return st->size;                                    \
    }                                                       \

#endif 
