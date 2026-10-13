#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>
#include <stdint.h>

typedef struct StringView StringView;

struct StringView {
    const char* ptr;
    size_t length;
};

void string_view_print(StringView* sv);
uint8_t string_view_strcmp(StringView* sv, const char* str);

#endif
