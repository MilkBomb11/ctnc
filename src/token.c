#include <stdio.h>
#include "token.h"
#include "stb_c_lexer.h"
#include "char_bank.h"

void token_print(Token* token)
{
   switch (token->token_code) {
      case CLEX_id        : printf("ID(%.*s)", (int)token->sv.length, token->sv.ptr); break;
      case CLEX_eq        : printf("=="); break;
      case CLEX_noteq     : printf("!="); break;
      case CLEX_lesseq    : printf("<="); break;
      case CLEX_greatereq : printf(">="); break;
      case CLEX_andand    : printf("&&"); break;
      case CLEX_oror      : printf("||"); break;
      case CLEX_shl       : printf("<<"); break;
      case CLEX_shr       : printf(">>"); break;
      case CLEX_plusplus  : printf("++"); break;
      case CLEX_minusminus: printf("--"); break;
      case CLEX_arrow     : printf("->"); break;
      case CLEX_andeq     : printf("&="); break;
      case CLEX_oreq      : printf("|="); break;
      case CLEX_xoreq     : printf("^="); break;
      case CLEX_pluseq    : printf("+="); break;
      case CLEX_minuseq   : printf("-="); break;
      case CLEX_muleq     : printf("*="); break;
      case CLEX_diveq     : printf("/="); break;
      case CLEX_modeq     : printf("%%="); break;
      case CLEX_shleq     : printf("<<="); break;
      case CLEX_shreq     : printf(">>="); break;
      case CLEX_eqarrow   : printf("=>"); break;
      case CLEX_dqstring  : printf("%.*s", (int)token->sv.length, token->sv.ptr); break;
      case CLEX_sqstring  : printf("%.*s", (int)token->sv.length, token->sv.ptr); break;
      case CLEX_charlit   : printf("%.*s", (int)token->sv.length, token->sv.ptr); break;
      #if defined(STB__clex_int_as_double) && !defined(STB__CLEX_use_stdlib)
      case CLEX_intlit    : printf("INT(%g)", token->real_number); break;
      #else
      case CLEX_intlit    : printf("INT(%ld)", token->int_number); break;
      #endif
      case CLEX_floatlit  : printf("REAL(%g)", token->real_number); break;
      default:
         if (token->token_code >= 0 && token->token_code< 256)
            printf("%c", (int) token->token_code);
         else {
            printf("<<<UNKNOWN TOKEN %ld >>>\n", token->token_code);
         }
         break;
   }
}

const char* token_code_to_string(long code)
{
   switch (code) {
      case CLEX_id        : return "id";
      case CLEX_eq        : return "==";
      case CLEX_noteq     : return "!=";
      case CLEX_lesseq    : return "<=";
      case CLEX_greatereq : return ">=";
      case CLEX_andand    : return "&&";
      case CLEX_oror      : return "||";
      case CLEX_shl       : return "<<";
      case CLEX_shr       : return ">>";
      case CLEX_plusplus  : return "++";
      case CLEX_minusminus: return "--";
      case CLEX_arrow     : return "->";
      case CLEX_andeq     : return "&=";
      case CLEX_oreq      : return "|=";
      case CLEX_xoreq     : return "^=";
      case CLEX_pluseq    : return "+=";
      case CLEX_minuseq   : return "-=";
      case CLEX_muleq     : return "*=";
      case CLEX_diveq     : return "/=";
      case CLEX_modeq     : return "%%=";
      case CLEX_shleq     : return "<<=";
      case CLEX_shreq     : return ">>=";
      case CLEX_eqarrow   : return "=>";
      case CLEX_dqstring  : return "double quote string";
      case CLEX_sqstring  : return "single quote string";
      case CLEX_charlit   : return "char literal";
      #if defined(STB__clex_int_as_double) && !defined(STB__CLEX_use_stdlib)
      case CLEX_intlit    : return "int literal";
      #else
      case CLEX_intlit    : return "int literal";
      #endif
      case CLEX_floatlit  : return "float literal";
      default:
         if (code >= 0 && code < 256) { return char_bank[code]; }
         else {
            printf("<<<UNKNOWN TOKEN %ld >>>\n", code);
            return "";
         }
   }
}
