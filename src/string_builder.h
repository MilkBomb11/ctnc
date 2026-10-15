#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stddef.h>
#include "string_view.h"

typedef struct StringBuilder StringBuilder;

struct StringBuilder {
    char* data;  
    size_t length;
    size_t capacity;
};

StringBuilder string_builder_create();
void string_builder_push_char(StringBuilder* sb, char c);
void string_builder_insert_number(StringBuilder* sb, size_t n, size_t index);
void string_builder_push_cstring(StringBuilder* sb, const char* str);
void string_builder_push_eat_sb(StringBuilder* sb_dest, StringBuilder* sb_src);
void string_builder_insert_char(StringBuilder* sb, char c, size_t index);
void string_builder_push_sv(StringBuilder* sb, StringView sv);
void string_builder_free(StringBuilder* sb);
void string_builder_print(StringBuilder* sb);

#endif
