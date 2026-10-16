#ifndef PRINTER_H
#define PRINTER_H

#include<stdio.h>

typedef struct StringViewRepo StringViewRepo;

typedef struct Printer Printer;
struct Printer {
    StringViewRepo* replace_srcs;
    StringViewRepo* replace_dests;
    const char* text;
    size_t len;
    FILE* out_file;
};

void printer_init(Printer* printer,
                  StringViewRepo* replace_srcs,
                  StringViewRepo* replace_dests,
                  const char* text,
                  size_t len,
                  FILE* out_file);
void printer_print(Printer* printer);

#endif
