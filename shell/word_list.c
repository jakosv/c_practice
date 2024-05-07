#include "word_list.h"

#include <string.h>
#include <stdlib.h>

void word_list_init(word_list_t *list)
{
    list->first = NULL;
    list->last = NULL;
}

void word_list_destroy(word_list_t *list)
{
    word_list_item_t *tmp;

    while (list->first) {
        tmp = list->first;
        list->first = list->first->next;
        free(tmp->word);
        free(tmp);
    } 
    list->last = NULL;
}

void word_list_clear(word_list_t *list)
{
    word_list_destroy(list);
    word_list_init(list);
}

void word_list_add(char *word, word_list_t *list)
{
    if (!list->first) {
        list->first = malloc(sizeof(word_list_item_t));
        list->last = list->first; 
    } else {
        list->last->next = malloc(sizeof(word_list_item_t));
        list->last = list->last->next;
    }
    list->last->word = word;
    list->last->next = NULL;
}
