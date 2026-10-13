#include <stdio.h>
#include "char_bank.h"

char char_bank[CHAR_BANK_LEN][2];

void char_bank_init()
{
    for (size_t i = 0; i < CHAR_BANK_LEN; i++) {
        sprintf(char_bank[i], "%c", (char)i);
        char_bank[i][1] = '\0';
    }
}
