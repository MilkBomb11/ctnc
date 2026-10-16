#ifndef PATHS_H
#define PATHS_H

#include <stddef.h>

typedef struct Paths Paths;
struct Paths {
    size_t size;
    size_t capacity;
    const char** data;
};

Paths paths_create();
void paths_push(Paths* paths, const char* path);
void paths_print(Paths* paths);
void paths_free(Paths* paths);

#endif
