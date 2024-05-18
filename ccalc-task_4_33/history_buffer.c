#include "history_buffer.h"

void history_init(history_buffer_t *hist)
{
    hist->size = 0;
}

int history_add(int n, history_buffer_t *hist)
{
    if (hist->size >= history_buflen)
        return 1;

    hist->buf[hist->size] = n;
    hist->size += 1;

    return 0;
}

int history_get(int idx, const history_buffer_t *hist)
{
    if (idx > hist->size)
        return 0;
    return hist->buf[idx];
}

void history_clear(history_buffer_t *hist)
{
    hist->size = 0;
}

void history_destroy(int n, history_buffer_t *hist)
{
    hist->size = 0;
}
