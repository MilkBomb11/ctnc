#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

void string_builder_insert_number(StringBuilder* sb, size_t n, size_t index)
{
    if (n == 0) {
        string_builder_insert_char(sb, '0', index);
        return;
    }

    while (n > 0) {
        char c = '0' + (n % 10);
        n /= 10;
        string_builder_insert_char(sb, c, index);
    }
}

void string_builder_push_cstring(StringBuilder* sb, const char* str)
{
    for (size_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        string_builder_push_char(sb, c);
    }
}

void string_builder_push_sv(StringBuilder* sb, StringView sv)
{
    for (size_t i = 0; i < sv.length; i++) {
        char c = sv.ptr[i];
        string_builder_push_char(sb, c);
    }  
}

void string_builder_push_eat_sb(StringBuilder* sb_eater, StringBuilder* sb_eaten)
{
    for (size_t i = 0; i < sb_eaten->length; i++) {
        char c = sb_eaten->data[i];
        string_builder_push_char(sb_eater, c);
    }
    string_builder_free(sb_eaten);
}

void string_builder_insert_char(StringBuilder* sb, char c, size_t index)
{
    if (index > sb->length) {
        printf("String builder tried to insert out of bound\n");
        exit(1);
    }
    string_builder_push_char(sb, 'T');
    for (size_t i = sb->length-1; i >= index+1; i--) {
        sb->data[i] = sb->data[i-1];
    }
    sb->data[index] = c;
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
