#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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


StringViewRepo string_view_repo_create()
{
    StringViewRepo svr;    
    svr.capacity = 4;
    svr.size = 0;
    svr.data = (StringView*)malloc(svr.capacity * sizeof(StringView));
    return svr;
}

void string_view_repo_push(StringViewRepo* svr, StringView sv)
{
    if (svr->size >= svr->capacity) {
        svr->capacity *= 2;
        StringView* temp = (StringView*)realloc(svr->data, svr->capacity * sizeof(StringView));
        if (temp == NULL) {
            printf("Reallocation for svr failed.\n");
            exit(1);
        }
        svr->data = temp;
    }
    svr->data[svr->size] = sv;
    svr->size++;
}

void string_view_repo_print(StringViewRepo* svr)
{
    printf("---BEGIN SVR CONTENTS---\n");
    for (size_t i = 0; i < svr->size; i++) {
        string_view_print(&svr->data[i]);
        printf("\n");
    }
    printf("---END SVR CONTENTS---\n");
}

void string_view_repo_free(StringViewRepo* svr)
{
    free(svr->data);
}
