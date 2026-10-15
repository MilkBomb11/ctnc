#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "args.h"
#include "string_view.h"
#include "injection_points.h"
#include "string_builder.h"


Type* ast_make_atom(StringView name)
{
    Type* type = (Type*)malloc(sizeof(Type));  
    type->kind = TT_ATOM;
    type->as.atom.name = name;
    return type;
}

Type* ast_make_ptr(Type* inner)
{
    Type* type = (Type*)malloc(sizeof(Type));   
    type->kind = TT_PTR;
    type->as.ptr.inner = inner;
    return type;
}

Type* ast_make_static(Type* inner)
{
    Type* type = (Type*)malloc(sizeof(Type));   
    type->kind = TT_STATIC;
    type->as.ptr.inner = inner;
    return type;
}

Type* ast_make_const(Type* inner)
{
    Type* type = (Type*)malloc(sizeof(Type));   
    type->kind = TT_CONST;
    type->as.ptr.inner = inner;
    return type;
}

Type* ast_make_struct(Type* inner)
{
    Type* type = (Type*)malloc(sizeof(Type));   
    type->kind = TT_STRUCT;
    type->as.ptr.inner = inner;
    return type;
}

Type* ast_make_arr(Type* elem, size_t size)
{
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = TT_ARR;
    type->as.arr.elem = elem;
    type->as.arr.size = size;
    return type;
}

Type* ast_make_func(Args args, Type* ret)
{
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = TT_FUNC;
    type->as.func.args = args;
    type->as.func.ret = ret;
    return type;
}

NameTypePair ast_make_ntp(StringView name, Type* type)
{
    NameTypePair ntp;
    ntp.name = name;
    ntp.type = type;
    return ntp;
}

void ast_free_type(Type* type)
{
    switch (type->kind) {
        case TT_ATOM:
            free(type);
            break;
        case TT_PTR:
            ast_free_type(type->as.ptr.inner);
            free(type);
            break;
        case TT_CONST:
            ast_free_type(type->as.const_.inner);
            free(type);
            break;
        case TT_STATIC:
            ast_free_type(type->as.static_.inner);
            free(type);
            break;
        case TT_STRUCT:
            ast_free_type(type->as.struct_.inner);
            free(type);
            break;
        case TT_ARR:
            ast_free_type(type->as.arr.elem);
            free(type);
            break;
        case TT_FUNC:
            args_free(&type->as.func.args);
            free(type);
            break;
        default:
            assert(0 && "unreachable");
    }
}

void ast_print_type(Type* type)
{
    switch (type->kind) {
        case TT_ATOM:
            printf("%.*s", (int)type->as.atom.name.length, type->as.atom.name.ptr);
            break;
        case TT_PTR:
            printf("ptr<");
            ast_print_type(type->as.ptr.inner);
            printf(">");
            break;
        case TT_CONST:
            printf("const<");
            ast_print_type(type->as.const_.inner);
            printf(">");
            break;
        case TT_STATIC:
            printf("static<");
            ast_print_type(type->as.static_.inner);
            printf(">");
            break;
        case TT_STRUCT:
            printf("struct<");
            ast_print_type(type->as.struct_.inner);
            printf(">");
            break;
        case TT_ARR:
            printf("arr<");
            ast_print_type(type->as.arr.elem);
            printf(", %zu>", type->as.arr.size);
            break;
        case TT_FUNC:
            printf("(");
            for (size_t i = 0; i < type->as.func.args.size; i++) {
                if (i != 0) { printf(", ");  }
                string_view_print(&type->as.func.args.data[i].name);
                printf(": ");
                ast_print_type(type->as.func.args.data[i].type);
            }
            printf(") -> ");
            ast_print_type(type->as.func.ret);
            break;
        default:
            assert(0 && "unreachable");
    }
}

void ast_print_ntp(NameTypePair* ntp)
{
    StringView name = ntp->name;
    Type* t = ntp->type;
    printf("%.*s: ", (int)name.length, name.ptr);
    ast_print_type(t);
}

InjectionPoints ast_translate_to_c(StringView name, Type* type, StringBuilder* sb)
{
    switch (type->kind) {
        case TT_ATOM: {
                InjectionPoints injections;
                string_builder_push_sv(sb, type->as.atom.name);
                string_builder_push_char(sb, ' ');
                injections.front = sb->length;
                string_builder_push_sv(sb, name);
                injections.back = sb->length;
                return injections;
            }
        case TT_STRUCT: {
                InjectionPoints injections;
                string_builder_push_cstring(sb, "struct");
                string_builder_push_char(sb, ' ');
                injections = ast_translate_to_c(name, type->as.struct_.inner, sb);
                return injections;
            }
        case TT_CONST: {
                InjectionPoints injections;
                string_builder_push_cstring(sb, "const");
                string_builder_push_char(sb, ' ');
                injections = ast_translate_to_c(name, type->as.const_.inner, sb);
                return injections;
            }
        case TT_STATIC: {
                InjectionPoints injections;
                string_builder_push_cstring(sb, "static");
                string_builder_push_char(sb, ' ');
                injections = ast_translate_to_c(name, type->as.static_.inner, sb);
                return injections;
            }
        case TT_PTR: {
                switch (type->as.ptr.inner->kind) {
                    case TT_ARR:
                    case TT_FUNC: {
                            InjectionPoints injections;
                            injections = ast_translate_to_c(name, type->as.ptr.inner, sb);
                            string_builder_insert_char(sb, ')', injections.back);
                            string_builder_insert_char(sb, '*', injections.front);
                            string_builder_insert_char(sb, '(', injections.front);
                            return injections;
                        }
                    default: {
                            InjectionPoints injections;
                            injections = ast_translate_to_c(name, type->as.ptr.inner, sb);
                            string_builder_insert_char(sb,'*', injections.front);
                            injections.front++;
                            injections.back++;
                            return injections;
                        }
                }
            }
        case TT_ARR: {
                InjectionPoints injections;
                injections = ast_translate_to_c(name, type->as.arr.elem, sb);
                string_builder_insert_char(sb, ']', injections.back);
                string_builder_insert_number(sb, type->as.arr.size, injections.back);
                string_builder_insert_char(sb, '[', injections.back);
                return injections;
            }
        case TT_FUNC: {
                InjectionPoints injections;
                injections = ast_translate_to_c(name, type->as.func.ret, sb);
                string_builder_push_char(sb, '(');
                for (size_t i = 0; i < type->as.func.args.size; i++) {
                    if (i != 0) { string_builder_push_cstring(sb, ", ");  }
                    StringView arg_name = type->as.func.args.data[i].name;
                    Type* arg_type = type->as.func.args.data[i].type;
                    ast_translate_to_c(arg_name, arg_type, sb);
                }
                string_builder_push_char(sb, ')');
                return injections;
            }
        default:
            assert(0 && "Unreachable.");
    }
}
