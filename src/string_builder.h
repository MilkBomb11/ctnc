#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stddef.h>

typedef struct StringBuilder StringBuilder;

struct StringBuilder {
    char* data;  
    size_t length;
    size_t capacity;
};

StringBuilder string_builder_create();
void string_builder_push_char(StringBuilder* sb, char c);
void string_builder_push_eat_sb(StringBuilder* sb_dest, StringBuilder* sb_src);
void string_builder_free(StringBuilder* sb);
void string_builder_print(StringBuilder* sb);

#endif
