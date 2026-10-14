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

typedef struct StringViewRepo StringViewRepo;
struct StringViewRepo {
    StringView* data;
    size_t size;
    size_t capacity;
};

StringViewRepo string_view_repo_create();
void string_view_repo_push(StringViewRepo* svr, StringView sv);
void string_view_repo_print(StringViewRepo* svr);
void string_view_repo_free(StringViewRepo* svr);

#endif
