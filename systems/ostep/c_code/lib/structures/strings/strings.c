#include "strings.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t char_pointer_length(char* pointer, size_t max_length) {
  size_t index = 0;
  while (pointer[index] != 0 && index < max_length - 1) {
    index++;
  }
  return index;
}

char debug_mode = 0;

size_t strings_created = 0;
string_t* string_create(char* data, size_t capacity) {
  string_t* result = malloc(sizeof(string_t) + capacity);
  if (!result) {
    fprintf(stderr, "failed to create string\n");
    exit(1);
  }

  size_t index = 0;
  for (index = 0; data[index] != 0 && index < capacity; index++) {
    result->data[index] = data[index];
  }
  result->capacity = capacity;
  result->length = index;

  if (debug_mode) {
    printf(
        "string_create:\n"
        "String pointer: \"%p\"\n"
        "Char pointer: \"%p\"\n"
        "Char pointer content: \"%s\"\n"
        "Capacity: %zu\n"
        "Length: %zu\n"
        "\n",
        result, result->data, result->data, result->capacity, result->length);
  }
  strings_created++;
  return result;
}

void string_internal_set(string_t* dest, char* data, size_t length) {
  size_t index = 0;
  size_t dest_capacity = dest->capacity;
  char* dest_data = dest->data;
  while ((index < length && index < dest_capacity) || data[index] != 0) {
    dest_data[index] = data[index];
    index++;
  }
  dest->length = length > dest_capacity ? dest_capacity : length;
}

void string_set_char(string_t* string, char* data, size_t length) {
  return string_internal_set(string, data, length);
}

void string_set(string_t* dest, string_t* src) {
  return string_internal_set(dest, src->data, src->length);
}

string_t* string_resize(string_t* string, size_t capacity) {
  if (!capacity || capacity == string->capacity) {
    return 0;
  }

  string = realloc(string, sizeof(string_t) + capacity);
  if (!string) {
    fprintf(stderr, "failed to resize\n");
    exit(1);
  }
  if (capacity < string->length) {
    string->length = capacity;
  }
  string->capacity = capacity;
  return string;
}

string_t* string_duplicate(string_t* string, size_t capacity) {
  return string_create(string->data, capacity);
}

size_t strings_disposed = 0;
void string_dispose(string_t** p) {
  if (!p) {
    return;
  }

  string_t* string = *p;
  if (!string) {
    return;
  }

  *p = 0;
  free(string);

  strings_disposed++;
  if (debug_mode) {
    printf(
        "string_dispose:\n"
        "String pointer: \"%p\"\n"
        "Char pointer: \"%p\"\n"
        "\n",
        string, string->data);
  }
}

void string_dispose_n(string_t** strings, size_t length) {
  while (length) {
    string_dispose(&strings[length - 1]);
    length--;
  }
}

char string_includes(string_t* string, string_t* search_string) {
  if (!search_string->length) {
    return 1;
  }

  if (search_string->length > string->length) {
    return 0;
  }

  size_t left_index = 0;
  size_t right_index = 0;
  while (left_index < string->length) {
    if (right_index == search_string->length) {
      return 1;
    }

    if (string->data[left_index] == search_string->data[right_index]) {
      right_index++;
    } else {
      right_index = 0;
    }

    left_index++;
  }

  if (right_index == search_string->length) {
    return 1;
  }

  return 0;
}

string_t* string_concat(string_t* left_string, string_t* right_string) {
  string_t* result = string_duplicate(left_string, left_string->capacity);
  if (result->capacity < result->length + right_string->length) {
    result = string_resize(result, result->length + right_string->length);
  }
  size_t index = 0;
  while (index < right_string->length) {
    result->data[result->length + index] = right_string->data[index];
    index++;
  }
  result->length += right_string->length;
  if (debug_mode) {
    printf(
        "string_concat:\n"
        "String pointer: \"%p\"\n"
        "Char pointer: \"%p\"\n"
        "Char pointer content: \"%s\"\n"
        "Capacity: %zu\n"
        "Length: %zu\n"
        "\n",
        result, result->data, result->data, result->capacity, result->length);
  }

  return result;
}

string_t* string_concat_n(string_t* strings[], size_t length) {
  if (!length) {
    return string_create("", 0);
  }

  if (length == 1) {
    return string_duplicate(strings[0], strings[0]->capacity);
  }

  string_t* result_string = string_duplicate(strings[0], strings[0]->capacity);
  size_t index = 1;
  while (index < length) {
    string_t* prev_string = result_string;
    result_string = string_concat(result_string, strings[index]);
    string_dispose(&prev_string);
    index++;
  }
  return result_string;
}

void string_once_spaces(string_t* string) {
  size_t string_length = string->length;
  if (string_length < 2) {
    return;
  }

  size_t index = 1;
  char* data = string->data;
  size_t chars_removed = 0;
  while (index < string_length) {
    char current_char = data[index];
    char prev_char = data[index - 1];
    if (current_char == ' ' && prev_char == ' ') {
      chars_removed++;
      index++;
      continue;
    }

    data[index - chars_removed] = current_char;
    index++;
  }
  string->length = string_length - chars_removed;
}

string_t** string_split_char(string_t* string, char c, size_t* buffer_length) {
  size_t string_length = string->length;
  if (!string_length) {
    *buffer_length = 0;
    return 0;
  }

  string_t** buffer = malloc(sizeof(string_t*) * (string_length + 1));
  char* data = string->data;
  size_t buffer_ret_length = 1;
  size_t index = 0;
  size_t current_start_index = 0;
  while (index < string_length) {
    char current_char = data[index];
    if (current_char == c) {
      buffer_ret_length++;
      size_t substring_length = index - current_start_index;
      string_t* substring =
          string_create(data + current_start_index, substring_length);
      buffer[buffer_ret_length - 2] = substring;
      if (index != string_length - 1) {
        current_start_index = index + 1;
      } else {
        current_start_index = string_length;
      }
    }

    index++;
  }

  string_t* end_string = string_create(data + current_start_index,
                                       string_length - current_start_index);
  buffer[buffer_ret_length - 1] = end_string;
  buffer = realloc(buffer, sizeof(string_t*) * buffer_ret_length);
  *buffer_length = buffer_ret_length;
  return buffer;
}

void string_trim_start(string_t* string) {
  if (!string->length) {
    return;
  }

  char* data = string->data;
  size_t string_length = string->length;
  size_t count = 0;
  while (data[count] == ' ') {
    count++;
  }

  if (!count) {
    return;
  }

  for (size_t index = count; index < string_length; index++) {
    data[index - count] = data[index];
  }

  string->length -= count;
}

void string_trim_end(string_t* string) {
  if (!string->length) {
    return;
  }

  char* data = string->data;
  size_t string_length = string->length;
  size_t count = 0;
  while (data[string_length - 1 - count] == ' ') {
    count++;
  }

  string->length -= count;
}

void string_trim(string_t* string) {
  string_trim_start(string);
  string_trim_end(string);
}

string_t* string_join_char(string_t** strings, size_t length, char separator) {
  if (!length) {
    return string_create("", 0);
  }

  if (length == 1) {
    return string_duplicate(strings[0], strings[0]->capacity);
  }

  char separator_null_terminated[2] = {separator, 0};
  size_t separators_length = length - 1;
  size_t index = 0;
  string_t* strings_to_concat[length + separators_length];
  while (index < length) {
    strings_to_concat[index * 2] = strings[index];
    index++;
  }
  index = 0;
  while (index < separators_length) {
    strings_to_concat[index * 2 + 1] =
        string_create(separator_null_terminated, 1);
    index++;
  }

  string_t* result_string =
      string_concat_n(strings_to_concat, length + separators_length);
  return result_string;
}

char string_internal_equal(string_t* string, char* pointer,
                           size_t pointer_length) {
  size_t string_length = string->length;
  if (string_length != pointer_length || (!string_length && pointer_length) ||
      (string_length && !pointer_length)) {
    return 0;
  }

  if (!string_length && !pointer_length) {
    return 1;
  }

  char* data = string->data;
  for (size_t index = 0; index < string_length; index++) {
    if (data[index] != pointer[index]) {
      return 0;
    }
  }
  return 1;
}

char string_equal_null_terminated(string_t* string, char* pointer,
                                  size_t pointer_length) {
  return string_internal_equal(string, pointer, pointer_length);
}

char string_equal(string_t* left_string, string_t* right_string) {
  return string_internal_equal(left_string, right_string->data,
                               right_string->length);
}

char string_starts_with_char(string_t* string, char c) {
  if (!string->length) {
    return 0;
  }

  return string->data[0] == c;
}

char string_ends_with_char(string_t* string, char c) {
  if (!string->length) {
    return 0;
  }

  return string->data[string->length - 1] == c;
}

char* string_to_null_terminated(string_t* string) {
  char* pointer = malloc(string->length + 1);
  size_t string_length = string->length;
  char* data = string->data;
  size_t index = 0;
  while (index < string_length) {
    pointer[index] = data[index];
    index++;
  }
  pointer[index] = 0;
  return pointer;
}

void string_append(string_t* dest, string_t* src) {
  size_t dest_length = dest->length;
  size_t dest_capacity = dest->capacity;
  if (dest_length == dest_capacity) {
    return;
  }

  size_t src_length = src->length;
  size_t dest_remaning_capacity = dest_capacity - dest_length;
  char* dest_data = dest->data;
  char* src_data = src->data;
  size_t index = 0;
  while (index < src_length && index < dest_remaning_capacity) {
    dest_data[dest_length + index] = src_data[index];
    index++;
  }

  if (index == src_length) {
    dest->length += src_length;
  } else {
    dest->length = dest->capacity;
  }
}

void string_append_char(string_t* dest, char c) {
  size_t dest_length = dest->length;
  size_t dest_capacity = dest->capacity;
  if (dest_length == dest_capacity) {
    return;
  }

  dest->data[dest_length] = c;
  dest->length++;
}
