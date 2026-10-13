#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "string_view.h"

uint8_t string_view_strcmp(StringView* sv, const char* str)
{
    return strncmp(sv->ptr, str, sv->length);
}

void string_view_print(StringView* sv)
{
    printf("%.*s", (int)sv->length, sv->ptr);
}
