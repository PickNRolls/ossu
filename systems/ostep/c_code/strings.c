#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct string_struct {
  char* pointer;
  uint32_t length;
  uint32_t capacity;
} string_t;

uint32_t char_pointer_length(char* pointer, uint32_t max_length) {
  uint32_t index = 0;
  while (pointer[index] != 0 && index < max_length - 1) {
    index++;
  }
  return index;
}

string_t* string_create(char* pointer, size_t capacity) {
  string_t* result = malloc(sizeof(string_t));

  result->pointer = malloc(capacity);
  size_t index = 0;
  while (pointer[index] != 0 && index < capacity - 1) {
    result->pointer[index] = pointer[index];
    index++;
  }
  result->capacity = capacity;
  result->length = index;
  printf(
      "string_create:\nString content: \"%s\"\nString pointer: %p\nString "
      "length: %i\n\n",
      result->pointer, result->pointer, result->length);
  return result;
}

void string_resize(string_t* string, size_t capacity) {
  if (capacity < string->capacity) {
    return;
  }

  string->capacity = capacity;
  string->pointer = realloc(string->pointer, capacity);
}

string_t* string_copy(string_t* string) {
  return string_create(string->pointer, string->capacity);
}

void string_dispose(string_t* string) {
  free(string->pointer);
  free(string);
}

int string_includes(string_t* string, string_t* search_string) {
  if (!search_string->length) {
    return 1;
  }

  if (search_string->length > string->length) {
    return 0;
  }

  uint32_t left_index = 0;
  uint32_t right_index = 0;
  while (left_index < string->length) {
    if (right_index == search_string->length) {
      return 1;
    }

    if (string->pointer[left_index] == search_string->pointer[right_index]) {
      right_index++;
    } else {
      right_index = 0;
    }

    left_index++;
  }

  return 0;
}

string_t* string_concat(string_t* left_string, string_t* right_string) {
  string_t* result = string_copy(left_string);
  if (result->capacity < result->length + right_string->length) {
    string_resize(result, result->capacity + right_string->length + 1);
  }
  size_t index = 0;
  while (index < right_string->length) {
    result->pointer[result->length + index] = right_string->pointer[index];
    index++;
  }
  result->pointer[result->length + index] = '\0';
  result->length += right_string->length;
  printf(
      "string_concat:\nString content: \"%s\"\nString pointer: %p\nString "
      "length: %i\n\n",
      result->pointer, result->pointer, result->length);
  return result;
}

int main(int argc, char* argv[]) {
  string_t* string = string_create("Marina loves me", 17);
  string_t* me_string = string_create("me", 3);
  int string_includes_me = string_includes(string, me_string);
  printf("%i\n", string_includes_me);
  string_dispose(string);
  string_dispose(me_string);
  return 0;
}
