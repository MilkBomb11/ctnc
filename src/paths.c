#include <stdio.h>
#include <stdlib.h>
#include "paths.h"

Paths paths_create()
{
    Paths paths;
    paths.capacity = 4;
    paths.size = 0;
    paths.data = (const char**)malloc(sizeof(const char*)*paths.capacity);
    return paths;
}

void paths_push(Paths* paths, const char* path)
{
    if (paths->size >= paths->capacity) {
        paths->capacity *= 2;
        const char** temp = realloc(paths->data, sizeof(const char*)*paths->capacity);
        if (temp == NULL) {
            printf("Realloc failed on paths.\n");
            exit(1);
        }
        paths->data = temp;
    }
    paths->data[paths->size] = path;
    paths->size++;
}
void paths_print(Paths* paths)
{
    for (size_t i = 0; i < paths->size; i++) {
        printf("%s\n", paths->data[i]);
    }
}
void paths_free(Paths* paths)
{
    free(paths->data);    
}
