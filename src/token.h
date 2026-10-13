#ifndef TOKEN_H
#define TOKEN_H


#include "string_view.h"

typedef struct Token Token;

struct Token {
    StringView sv;  
    long token_code;
    double real_number;
    long   int_number;
};


void token_print(Token* token);
const char* token_code_to_string(long code);

#endif
