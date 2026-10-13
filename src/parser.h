#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include "stb_c_lexer.h"
#include "token.h"
#include "ast.h"

typedef struct Parser Parser;

struct Parser {
    stb_lexer lex;
    Token current;
    Token prev;
    uint8_t is_current_valid;
    uint8_t is_prev_valid;
    uint8_t is_at_end;
};

void parser_init(Parser* parser, char* text, int len);
int parser_advance(Parser* parser);
uint8_t parser_match(Parser* parser, long token_code);
void parser_eat(Parser* parser, long token_code);
void parser_free(Parser* parser);
Type* parser_parse_type(Parser* parser);
NameTypePair parser_parse_ntp(Parser* parser);
Args parser_parse_args(Parser* parser);
void parser_parse(Parser* parser, const char* text);

#endif
