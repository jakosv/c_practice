#ifndef WORD_LIST_H_SENTRY
#define WORD_LIST_H_SENTRY

typedef struct word_list_item {
    char *word;
    struct word_list_item *next;
} word_list_item_t;

typedef struct word_list {
    word_list_item_t *first, *last;
} word_list_t;

void word_list_init(word_list_t *list);
void word_list_destroy(word_list_t *list);
void word_list_clear(word_list_t *list);
void word_list_add(char *word, word_list_t *list);

#endif
