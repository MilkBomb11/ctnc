#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parser.h"
#include "args.h"
#include "string_builder.h"
#include "token.h"
#include "ast.h"
#include "string_view.h"

void parser_init(Parser* parser, char* text, int len)
{
    stb_c_lexer_init(&parser->lex, text, text+len, (char *) malloc(0x10000), 0x10000);
    parser->is_current_valid = 0;
    parser->is_prev_valid = 0;
}

int parser_advance(Parser* parser)
{
    int token_parsed = stb_c_lexer_get_token(&parser->lex);
    parser->is_at_end = 0;
    if (!token_parsed) {
        parser->is_at_end = 1;
        parser->prev = parser->current; 
        parser->is_prev_valid = parser->is_current_valid;
        return token_parsed;
    }

    parser->prev = parser->current; 
    parser->is_prev_valid = parser->is_current_valid;

    parser->current.token_code = parser->lex.token;
    parser->current.sv.ptr = parser->lex.where_firstchar;
    size_t length = parser->lex.where_lastchar - parser->lex.where_firstchar + 1;
    parser->current.sv.length = length;
    parser->current.int_number = parser->lex.int_number;
    parser->current.real_number = parser->lex.real_number;
    parser->is_current_valid = 1;
    return token_parsed;
}

uint8_t parser_match(Parser* parser, long token_code)
{
    if (parser->current.token_code == token_code) {
        parser_advance(parser);
        return 1;
    }
    return 0; 
}

void parser_eat(Parser* parser, long token_code)
{
    if (parser->current.token_code == token_code) {
        parser_advance(parser);
        return;
    }
    stb_lex_location location;
    stb_c_lexer_get_location(&parser->lex, parser->current.sv.ptr, &location);
    printf("[row: %d, col: %d]: Expected %s but got %s.",
            location.line_number,
            location.line_offset,
            token_code_to_string(token_code),
            token_code_to_string(parser->current.token_code));
    exit(1);
}

Type* parser_parse_type(Parser* parser)
{
    if (parser_match(parser, CLEX_id)) {
        if (string_view_strcmp(&parser->prev.sv, "ptr") == 0) {
            parser_eat(parser, '{');
            Type* t = ast_make_ptr(parser_parse_type(parser));
            parser_eat(parser, '}');
            return t;
        }
        else if (string_view_strcmp(&parser->prev.sv, "const") == 0) {
            parser_eat(parser, '{');
            Type* t = ast_make_const(parser_parse_type(parser));
            parser_eat(parser, '}');
            return t;
        }
        else if (string_view_strcmp(&parser->prev.sv, "struct") == 0) {
            parser_eat(parser, CLEX_id);
            return ast_make_struct(ast_make_atom(parser->prev.sv));
        }
        else if (string_view_strcmp(&parser->prev.sv, "static") == 0) {
            parser_eat(parser, '{');
            Type* t = ast_make_static(parser_parse_type(parser));
            parser_eat(parser, '}');
            return t;
        }
        else if (string_view_strcmp(&parser->prev.sv, "arr") == 0) {
            parser_eat(parser, '{');
            Type* elem = parser_parse_type(parser);
            parser_eat(parser, ',');
            parser_eat(parser, CLEX_intlit);
            int size = parser->prev.int_number;
            parser_eat(parser, '}');
            return ast_make_arr(elem, size);
        }
        else if (string_view_strcmp(&parser->prev.sv, "fn") == 0) {
            Args args = parser_parse_args(parser);
            parser_eat(parser, CLEX_arrow);
            Type* ret = parser_parse_type(parser);
            return ast_make_func(args, ret);
        }
        else {
            return ast_make_atom(parser->prev.sv);
        }
    }
    printf("parse_type expects identifier but got %s", token_code_to_string(parser->current.token_code));
    exit(1);
}

Args parser_parse_args(Parser* parser)
{
    Args args = args_create();
    parser_eat(parser, '(');
    if (parser_match(parser, ')')) {
        return args;
    }
    while (1) {
        NameTypePair ntp = parser_parse_ntp(parser);
        args_push(&args, ntp);
        if (parser_match(parser, ')')) {
            return args;
        }
        else {
            parser_eat(parser, ',');
        }
    }
    return args;
}

NameTypePair parser_parse_ntp(Parser* parser)
{
    parser_eat(parser, CLEX_id);
    Token id = parser->prev;
    parser_eat(parser, ':');
    Type* t = parser_parse_type(parser);
    return ast_make_ntp(id.sv, t);
}

void parser_parse(Parser* parser, StringViewRepo* replaces_dests, StringViewRepo* replace_srcs, const char* text)
{
    while (parser_advance(parser)) {
        if (parser_match(parser, CLEX_id)) {
            Token id = parser->prev;
            const char* start_ptr = id.sv.ptr;
            if (parser_match(parser, ':')) {
                Type* t = parser_parse_type(parser);

                const char* end_ptr = parser->prev.sv.ptr + parser->prev.sv.length;
                int length = end_ptr - start_ptr;
                StringView sv_dest;
                sv_dest.ptr = start_ptr;
                sv_dest.length = length;
                string_view_repo_push(replaces_dests, sv_dest);
                
                NameTypePair ntp;
                ntp.name = id.sv;
                ntp.type = t;

                StringBuilder sb = string_builder_create();
                ast_translate_to_c(ntp.name, ntp.type, &sb);

                StringView sv_src;
                sv_src.ptr = sb.data;
                sv_src.length = sb.length; // move sb's data to sv_src so don't free sb.
                string_view_repo_push(replace_srcs, sv_src);
                ast_free_type(t);
            }
        }
    }
}

void parser_free(Parser* parser)
{
    free(parser->lex.string_storage);
}
