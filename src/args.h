#ifndef ARGS_H
#define ARGS_H

#include <stddef.h>
typedef struct NameTypePair NameTypePair;


typedef struct Args Args;

struct Args {
    NameTypePair* data;
    size_t size;
    size_t capacity;
};

Args args_create();
void args_push(Args* args, NameTypePair ntp);
NameTypePair args_get(Args* args, size_t index);
void args_free(Args* args);

#endif
