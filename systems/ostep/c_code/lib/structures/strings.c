#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct string_struct {
  char* pointer;
  size_t length;
  size_t capacity;
} string_t;

size_t char_pointer_length(char* pointer, size_t max_length) {
  size_t index = 0;
  while (pointer[index] != 0 && index < max_length - 1) {
    index++;
  }
  return index;
}

char debug_mode = 0;

size_t strings_created = 0;
string_t* string_create(char* pointer, size_t capacity) {
  if (!capacity) {
    fprintf(stderr, "zero capacity for string_create");
    exit(1);
  }

  string_t* result = malloc(sizeof(string_t));
  if (!result) {
    fprintf(stderr, "failed to create string\n");
    exit(1);
  }

  result->pointer = malloc(capacity);
  if (!result->pointer) {
    fprintf(stderr, "failed to create string\n");
    exit(1);
  }
  size_t index = 0;
  while (pointer[index] != 0 && index < capacity - 1) {
    result->pointer[index] = pointer[index];
    index++;
  }
  result->pointer[index] = '\0';
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
        result, result->pointer, result->pointer, result->capacity,
        result->length);
  }
  strings_created++;
  return result;
}

void string_resize(string_t* string, size_t capacity) {
  if (!capacity || capacity == string->capacity) {
    return;
  }

  if (capacity < string->capacity) {
    string->pointer[capacity - 1] = '\0';
  }

  if (capacity <= string->length) {
    string->length = capacity - 1;
  }

  char* pointer = realloc(string->pointer, capacity);
  if (!pointer) {
    fprintf(stderr, "failed to resize\n");
    exit(1);
  }
  string->capacity = capacity;
  string->pointer = pointer;
}

string_t* string_copy(string_t* string) {
  return string_create(string->pointer, string->capacity);
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

  char* prev_string_pointer = string->pointer;
  string->pointer = 0;
  free(prev_string_pointer);
  *p = 0;
  free(string);

  strings_disposed++;
  if (debug_mode) {
    printf(
        "string_dispose:\n"
        "String pointer: \"%p\"\n"
        "Char pointer: \"%p\"\n"
        "\n",
        string, string->pointer);
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

    if (string->pointer[left_index] == search_string->pointer[right_index]) {
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
  string_t* result = string_copy(left_string);
  if (result->capacity < result->length + right_string->length + 1) {
    string_resize(result, result->length + right_string->length + 1);
  }
  size_t index = 0;
  while (index < right_string->length) {
    result->pointer[result->length + index] = right_string->pointer[index];
    index++;
  }
  result->pointer[result->length + index] = '\0';
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
        result, result->pointer, result->pointer, result->capacity,
        result->length);
  }

  return result;
}

string_t* string_concat_n(string_t* strings[], size_t length) {
  if (!length) {
    return string_create("", 1);
  }

  if (length == 1) {
    return string_copy(strings[0]);
  }

  string_t* result_string = string_copy(strings[0]);
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
  char* pointer = string->pointer;
  size_t chars_removed = 0;
  while (index < string_length) {
    char current_char = pointer[index];
    char prev_char = pointer[index - 1];
    if (current_char == ' ' && prev_char == ' ') {
      chars_removed++;
      index++;
      continue;
    }

    pointer[index - chars_removed] = current_char;
    index++;
  }
  pointer[string_length - chars_removed] = '\0';
  string->length = string_length - chars_removed;
}

string_t** string_split_char(string_t* string, char c, size_t* buffer_length) {
  string_t** buffer = malloc(sizeof(string_t*));
  char* pointer = string->pointer;
  size_t string_length = string->length;
  size_t buffer_ret_length = 1;
  size_t index = 0;
  size_t current_start_index = 0;
  while (index < string_length) {
    char current_char = pointer[index];
    if (current_char == c) {
      buffer_ret_length++;
      size_t substring_length = index - current_start_index;
      buffer = realloc(buffer, sizeof(string_t*) * buffer_ret_length);
      string_t* substring =
          string_create(pointer + current_start_index, substring_length + 1);
      buffer[buffer_ret_length - 2] = substring;
      if (index != string_length - 1) {
        current_start_index = index + 1;
      } else {
        current_start_index = string_length;
      }
    }

    index++;
  }

  string_t* end_string = string_create(pointer + current_start_index,
                                       string_length - current_start_index + 1);
  buffer[buffer_ret_length - 1] = end_string;
  *buffer_length = buffer_ret_length;
  return buffer;
}

void string_trim_start(string_t* string) {
  if (!string->length) {
    return;
  }

  char* pointer = string->pointer;
  size_t string_length = string->length;
  size_t index = 0;
  while (pointer[index] == ' ') {
    index++;
  }

  if (!index) {
    return;
  }

  size_t offset = index;
  while (index < string_length) {
    pointer[index - offset] = pointer[index];
    if (index >= string_length - offset) {
      pointer[index] = '\0';
    }
    index++;
  }

  string->length -= offset;
}

void string_trim_end(string_t* string) {
  if (!string->length) {
    return;
  }

  char* pointer = string->pointer;
  size_t string_length = string->length;
  size_t index = string_length - 1;
  while (pointer[index] == ' ') {
    pointer[index] = '\0';
    index--;
  }

  string->length -= string->length - index - 1;
}

void string_trim(string_t* string) {
  string_trim_start(string);
  string_trim_end(string);
}

string_t* string_join_char(string_t** strings, size_t length, char separator) {
  if (length < 2) {
    return string_create(&separator, 2);
  }

  size_t separators_length = length - 1;
  size_t index = 0;
  string_t* strings_to_concat[length + separators_length];
  while (index < length) {
    strings_to_concat[index * 2] = strings[index];
    index++;
  }
  index = 0;
  while (index < separators_length) {
    strings_to_concat[index * 2 + 1] = string_create(&separator, 2);
    index++;
  }

  string_t* result_string =
      string_concat_n(strings_to_concat, length + separators_length);
  string_dispose_n(strings_to_concat, length + separators_length);
  return result_string;
}
