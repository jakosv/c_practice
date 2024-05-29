#include "history_buffer.h"

void history_init(history_buffer_t *hist)
{
    hist->size = 0;
}

enum history_err history_add(int n, history_buffer_t *hist)
{
    if (hist->size >= history_buflen)
        return hs_full;

    hist->buf[hist->size] = n;
    hist->size += 1;

    return hs_ok;
}

enum history_err history_get(int idx, const history_buffer_t *hist, int *res)
{
    if (idx >= hist->size)
        return hs_out_of_range;
    *res = hist->buf[idx];
    return hs_ok;
}

enum history_err history_top(const history_buffer_t *hist, int *res)
{
    if (hist->size == 0)
        return hs_out_of_range;
    *res = hist->buf[hist->size - 1];
    return hs_ok;
}

int history_size(const history_buffer_t *hist)
{
    return hist->size;
}

void history_clear(history_buffer_t *hist)
{
    hist->size = 0;
}

void history_destroy(history_buffer_t *hist)
{
    hist->size = 0;
}
