#include <stdint.h>
#include <stdlib.h>

#ifndef LIB_STRUCTURES_STRINGS_H
#define LIB_STRUCTURES_STRINGS_H
typedef struct string_struct {
  char* pointer;
  size_t length;
  size_t capacity;
} string_t;

string_t* string_create(char* pointer, size_t capacity);
void string_dispose(string_t** pointer);
void string_dispose_n(string_t** strings, size_t length);
void string_resize(string_t* string, size_t capacity);
char string_includes(string_t* string, string_t* search_string);
string_t* string_concat(string_t* left_string, string_t* right_string);
string_t* string_concat_n(string_t** strings, size_t length);
void string_once_spaces(string_t* string);
string_t** string_split_char(string_t* string, char c, size_t* buffer_length);
void string_trim_start(string_t* string);
void string_trim_end(string_t* string);
void string_trim(string_t* string);
string_t* string_join_char(string_t** strings, size_t length, char separator);
#endif
