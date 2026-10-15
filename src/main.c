#include<stdio.h>

#define STB_C_LEXER_IMPLEMENTATION
#include"stb_c_lexer.h"
#undef STB_C_LEXER_IMPLEMENTATION

#include "parser.h"
#include "char_bank.h"
#include "string_view.h"

size_t get_file_size(FILE* f)
{
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);   
    fseek(f, 0, SEEK_SET);
    return file_size;
}

int main(int argc, char **argv)
{
    FILE* in_file = fopen("test/test.c","rb");
    size_t file_size = get_file_size(in_file);
    char* text = (char*) malloc(file_size);
    int len = in_file ? (int) fread(text, 1, file_size, in_file) : -1;
    if (len < 0) {
        fprintf(stderr, "Error opening file\n");
        free(text);
        fclose(in_file);
        return 1;
    }
    fclose(in_file);

    Parser parser;
    StringViewRepo svr = string_view_repo_create();

    parser_init(&parser, text, len);
    char_bank_init();
    parser_parse(&parser, &svr, text);
    string_view_repo_print(&svr);

    parser_free(&parser);
    string_view_repo_free(&svr);

    // StringBuilder test = string_builder_create();
    // string_builder_push_char(&test, 'A');
    // string_builder_push_char(&test, 'B');
    // string_builder_push_char(&test, 'C');
    // string_builder_push_char(&test, 'D');
    // string_builder_push_char(&test, 'E');
    // string_builder_insert_char(&test, '1', 0);
    // string_builder_insert_char(&test, '2', 4);
    // string_builder_insert_char(&test, '3', test.length);
    // string_builder_print(&test);
    return 0;
}
