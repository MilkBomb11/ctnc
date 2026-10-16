#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdint.h>
#include <stdio.h>

typedef struct Paths Paths;
typedef struct Flags Flags;
struct Flags {
    uint8_t is_stdout;
    uint8_t is_help;
};

int frontend_parse_file(const char* path, FILE* out_file);
int frontend_parse_files(int argc, char* argv[]);
void frontend_get_flags_and_args(Flags* flags, Paths* paths, int argc, char* argv[]);
void frontend_flags_init(Flags* flags);


#endif
