#include "printer.h"
#include "string_view.h"
#include <stdio.h>

void printer_init(Printer* printer,
                  StringViewRepo* replace_srcs,
                  StringViewRepo* replace_dests,
                  const char* text, size_t len,
                  FILE* out_file)
{
    printer->replace_dests = replace_dests;    
    printer->replace_srcs = replace_srcs;
    printer->text = text;
    printer->len = len;
    printer->out_file = out_file;
}

void printer_print(Printer* printer)
{
    size_t repo_index = 0;
    for (size_t i = 0; i < printer->len;) {
        StringView replace_dest = printer->replace_dests->data[repo_index];
        StringView replace_src = printer->replace_srcs->data[repo_index];
        if (i == replace_dest.ptr - printer->text) {
            fprintf(printer->out_file, "%.*s", (int)replace_src.length, replace_src.ptr);
            i += replace_dest.length;
            repo_index++;
        }
        else {
            fprintf(printer->out_file, "%c", printer->text[i]);
            i++;
        }
    }
}
