#define STB_C_LEXER_IMPLEMENTATION
#include"stb_c_lexer.h"
#undef STB_C_LEXER_IMPLEMENTATION

#include "frontend.h"

int main(int argc, char **argv)
{
    frontend_parse_files(argc, argv);
    return 0;
}
