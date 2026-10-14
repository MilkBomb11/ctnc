#include <stdio.h>
#include <stdlib.h>
#include "args.h"
#include "ast.h"

Args args_create()
{
    Args args;
    args.size = 0;
    args.capacity = 2;
    args.data = (NameTypePair*) malloc(args.capacity*sizeof(NameTypePair));
    return args;
}

void args_push(Args* args, NameTypePair ntp)
{
    if (args->size >= args->capacity) {
        args->capacity *= 2;
        NameTypePair* temp = realloc(args->data, args->capacity*sizeof(NameTypePair));
        if (temp == NULL) {
            printf("Realloc failed for args.");
            exit(1);
        }
        args->data = temp;
    }
    args->data[args->size] = ntp;
    args->size++;
}

NameTypePair args_get(Args* args, size_t index)
{
    if (index >= args->size) {
        printf("Array index out of bound. [ind = %zu, size = %zu]", index, args->size);
        exit(1);
    }
    return args->data[index];
}

void args_free(Args* args)
{
    for (size_t i = 0; i < args->size; i++) {
        ast_free_type(args->data[i].type);
    }
    free(args->data);
}
