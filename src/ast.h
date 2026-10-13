#ifndef AST_H
#define AST_H

#include<stddef.h>
#include "string_view.h"
#include "args.h"

typedef enum TypeKind {
    TT_ATOM,
    TT_PTR,
    TT_ARR,
    TT_CONST,
    TT_STATIC,
    TT_STRUCT,
    TT_FUNC,
} TypeKind;

typedef struct Type Type;
typedef struct NameTypePair NameTypePair;

struct Type {
    TypeKind kind;
    union {
        struct { StringView name; } atom;
        struct { Type* inner; } ptr;
        struct { Type* inner; } static_;
        struct { Type* inner; } const_;
        struct { Type* inner; } struct_;
        struct { Type* elem; size_t size; } arr;
        struct { Args args; Type* ret; } func;
    } as;
};

struct NameTypePair {
    StringView name;
    Type* type;
};

Type* ast_make_atom(StringView name);
Type* ast_make_ptr(Type* inner);
Type* ast_make_static(Type* inner);
Type* ast_make_const(Type* inner);
Type* ast_make_struct(Type* inner);
Type* ast_make_arr(Type* elem, size_t size);
Type* ast_make_func(Args args, Type* ret);

NameTypePair ast_make_ntp(StringView name, Type* type);

void ast_free_type(Type* type);
void ast_print_type(Type* type);
#endif
