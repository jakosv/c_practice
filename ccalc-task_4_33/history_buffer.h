#ifndef HISTORY_BUFFER_H_SENTRY
#define HISTORY_BUFFER_H_SENTRY

enum history_err { hs_ok, hs_out_of_range, hs_full };
enum { history_buflen = 2 };

typedef struct history_buffer {
    int size;
    int buf[history_buflen];
} history_buffer_t;

void history_init(history_buffer_t *hist);
int history_size(const history_buffer_t *hist);
enum history_err history_add(int n, history_buffer_t *hist);
enum history_err history_get(int idx, const history_buffer_t *hist, int *res);
enum history_err history_top(const history_buffer_t *hist, int *res);
void history_clear(history_buffer_t *hist);
void history_destroy(history_buffer_t *hist);

#endif
