#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "char_bank.h"
#include "string_builder.h"
#include "string_view.h"
#include "printer.h"
#include "frontend.h"
#include "paths.h"

const char* usage_string =
    "<path to ctnc> [-s | --stdout] [-h | --help] <path to file>...\n";

static size_t get_file_size(FILE* f)
{
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);   
    fseek(f, 0, SEEK_SET);
    return file_size;
}

int frontend_parse_file(const char* path, FILE* out_file)
{
    FILE* in_file = fopen(path,"rb");
    if (in_file == NULL) {
        printf("Failed to open file %s.\n", path);
        exit(1);
    }
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
    StringViewRepo replace_dests = string_view_repo_create(0);
    StringViewRepo replace_srcs = string_view_repo_create(1);

    char_bank_init();

    parser_init(&parser, text, len);
    parser_parse(&parser, &replace_dests, &replace_srcs, text);
    parser_free(&parser);

#ifdef DEBUG
    string_view_repo_print(&replace_dests);
    string_view_repo_print(&replace_srcs);
#endif

    Printer printer;    
    printer_init(&printer, &replace_srcs, &replace_dests, text, len, out_file);
    printer_print(&printer);
    
    string_view_repo_free(&replace_dests);
    string_view_repo_free(&replace_srcs);
    free(text);
    return 0;
}

void frontend_flags_init(Flags* flags) {
    flags->is_stdout = 0;
    flags->is_help = 0;
}

void frontend_get_flags_and_args(Flags* flags, Paths* paths, int argc, char* argv[])
{
    for (size_t i = 0; i < argc; i++) {
        const char* arg = argv[i];
        if (!strcmp(arg, "-s") || !strcmp(arg, "--stdout")) {
            flags->is_stdout = 1;            
        }
        else if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
            flags->is_help = 1;
        }
        else {
            paths_push(paths, arg);
        }
    }
}

int frontend_parse_files(int argc, char* argv[])
{
    Flags flags;
    frontend_flags_init(&flags);
    Paths paths = paths_create();
    frontend_get_flags_and_args(&flags, &paths, argc, argv);
    if (flags.is_help || paths.size == 0) {
        printf("%s\n", usage_string);
        return 0;
    }
    for (size_t i = 1; i < paths.size; i++) {
        const char* path = paths.data[i];
        if (flags.is_stdout) {
            frontend_parse_file(path, stdout);
            continue;
        }
        
        StringBuilder sb = string_builder_create();
        string_builder_push_cstring(&sb, path);
        
        StringView sv;
        sv.ptr = sb.data;
        sv.length = sb.length;

        const char* name_end = string_view_ends_with(&sv, ".ctnc");
        if (name_end) {
            sb.length = name_end - sb.data;
        }
        string_builder_push_cstring(&sb, ".c");
        string_builder_push_char(&sb, '\0');

        FILE* out_file = fopen(sb.data, "w");
        if (out_file == NULL) {
            printf("Failed to open %s.\n", sb.data);
            exit(1);
        }
        frontend_parse_file(path, out_file);
        string_builder_free(&sb);
    }

    paths_free(&paths);
    return 0;
}
