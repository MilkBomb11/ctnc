#include <stdio.h>
#include <stdlib.h>
#include "string_builder.h"
#include "string_view.h"


StringBuilder string_builder_create()
{
    StringBuilder sb;
    sb.capacity = 4;
    sb.length = 0;
    sb.data = (char*)malloc(sb.capacity * sizeof(char)); 
    return sb;
}

void string_builder_push_char(StringBuilder* sb, char c)
{
    if (sb->length >= sb->capacity) {
        sb->capacity *= 2;
        char* temp = realloc(sb->data, sb->capacity * sizeof(char));
        if (temp == NULL) {
            printf("Realloc failed for string builder");
            exit(1);
        }
        sb->data = temp;
    }
    sb->data[sb->length] = c;
    sb->length++;
}

void string_builder_push_eat_sb(StringBuilder* sb_eater, StringBuilder* sb_eaten)
{
    for (size_t i = 0; i < sb_eaten->length; i++) {
        char c = sb_eaten->data[i];
        string_builder_push_char(sb_eater, c);
    }
}

void string_builder_print(StringBuilder* sb)
{
    StringView sv;
    sv.ptr = sb->data;    
    sv.length = sb->length;
    string_view_print(&sv);
}

void string_builder_free(StringBuilder* sb)
{
    free(sb->data);    
}
