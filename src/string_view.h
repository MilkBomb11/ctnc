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
void string_view_free(StringView* sv);
const char* string_view_ends_with(StringView* sv, const char* str);

typedef struct StringViewRepo StringViewRepo;
struct StringViewRepo {
    StringView* data;
    size_t size;
    size_t capacity;
    uint8_t elem_is_owner;
};

StringViewRepo string_view_repo_create(uint8_t elem_is_owner);
void string_view_repo_push(StringViewRepo* svr, StringView sv);
void string_view_repo_print(StringViewRepo* svr);
void string_view_repo_free(StringViewRepo* svr);

#endif
