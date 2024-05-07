#ifndef WORD_PARSER_H_SENTRY
#define WORD_PARSER_H_SENTRY

enum parser_status { ps_ok, ps_empty, ps_eof, ps_eoln, ps_open_quote };

typedef struct word_parser word_parser_t;

word_parser_t *word_parser_init();
void word_parser_free(word_parser_t *parser);
enum parser_status word_parser_read(word_parser_t *parser);
char *word_parser_get_word_as_c_str(const word_parser_t *parser);

#endif
