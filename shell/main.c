#include "word_list.h"
#include "word_parser.h"

#include <stdio.h>
#include <stdlib.h>

static void words_print(const char *format, const word_list_t *words)
{
    word_list_item_t *tmp;
    for (tmp = words->first; tmp; tmp = tmp->next)
        printf(format, tmp->word);
}

static void add_parsed_word_to_list(const word_parser_t *parser,
                                    word_list_t *list)
{
    char *word = word_parser_get_word_as_c_str(parser);
    word_list_add(word, list); 
}

static void handle_end_of_line(const word_parser_t *parser, word_list_t *list)
{
    add_parsed_word_to_list(parser, list);
    words_print("[%s]\n", list);
    word_list_clear(list);
}

static void handle_unclosed_quote(word_list_t *list)
{
    fprintf(stderr, "Error: string contains unclosed quote\n");
    word_list_clear(list);
}

static int handle_parser_result(enum parser_status status, 
                                const word_parser_t *parser,
                                word_list_t *list)
{
    switch(status) {
    case ps_ok:
        add_parsed_word_to_list(parser, list);
        break;
    case ps_empty:
        break;
    case ps_eoln:
        handle_end_of_line(parser, list);
        break;
    case ps_open_quote:
        handle_unclosed_quote(list);
        break;
    case ps_eof:
        return 1;
    }
    return 0;
}

int main()
{
    word_list_t *list;
    word_parser_t *parser;

    list = malloc(sizeof(word_list_t));

    word_list_init(list);

    parser = word_parser_init();
    do {
        enum parser_status status;
        int res;
        status = word_parser_read(parser);
        res = handle_parser_result(status, parser, list);
        if (res)
            break;
    } while (1);
    word_parser_free(parser);

    word_list_destroy(list);
    free(list);

    return 0;
}
