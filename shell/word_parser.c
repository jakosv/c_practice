#include "word_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word_parser {
    char *word;
    int word_len, word_capacity;
    int quotes_mode, was_quotes;
};


enum { capacity_grow_coef = 2 };


word_parser_t *word_parser_init()
{
    word_parser_t *parser;

    parser = malloc(sizeof(*parser));
    parser->word_capacity = capacity_grow_coef;
    parser->word = malloc(capacity_grow_coef);
    parser->word[0] = '\0';
    parser->word_len = 0;
    parser->quotes_mode = 0;

    return parser;
}

void word_parser_free(word_parser_t *parser)
{
    free(parser->word);
    parser->word_len = 0;
    parser->word_capacity = 0;
    free(parser);
}

static void word_push_char(char ch, word_parser_t *parser)
{
    
    if (parser->word_len >= parser->word_capacity) {
        parser->word_capacity *= capacity_grow_coef;
        parser->word = realloc(parser->word, parser->word_capacity);
    }
    parser->word[parser->word_len] = ch;
    parser->word_len += 1;
}

static void word_parser_clear(word_parser_t *parser)
{
    parser->word_len = 0;
    parser->quotes_mode = 0;
    parser->was_quotes = 0;
}

static void read_word(word_parser_t *parser, int *last_char)
{
    int c;
    int backslash = 0;

    while ((c = getchar()) != EOF) {
        if (c == '\n')
            break;
        if (c == ' ' && !parser->quotes_mode) {
            if (parser->word_len > 0)
                break;
            else
                continue;
        }
        if (c == '"' && !backslash) {
            if (parser->quotes_mode) {
                parser->quotes_mode = 0;
                continue;
            }
            parser->quotes_mode = 1;
            parser->was_quotes = 1;
            continue;
        }
        if (c == '\\' && !parser->quotes_mode) {
            backslash = 1;
            continue;
        }
        backslash = 0;

        word_push_char(c, parser);
    }

    *last_char = c;
} 

enum parser_status word_parser_read(word_parser_t *parser)
{
    int last_char;

    word_parser_clear(parser);
    read_word(parser, &last_char);

    if (parser->quotes_mode)
        return ps_open_quote;

    if (parser->word_len > 0 || parser->was_quotes) {
        if (last_char == '\n')
            return ps_eoln;
        return ps_ok;
    }
    
    switch (last_char) {
    case EOF:
        return ps_eof;
    case '\n':
        return ps_empty;
    }

    return ps_ok;
}

char *word_parser_get_word_as_c_str(const word_parser_t *parser)
{
    char *str;

    str = malloc(parser->word_len + 1);
    strncpy(str, parser->word, parser->word_len);
    str[parser->word_len] = '\0';

    return str;
}
