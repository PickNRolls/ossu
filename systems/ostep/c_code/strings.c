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

char debug_mode = 0;

size_t strings_created = 0;
string_t* string_create(char* pointer, size_t capacity) {
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
        "Capacity: %i\n"
        "Length: %i\n"
        "\n",
        result, result->pointer, result->pointer, result->capacity,
        result->length);
  }
  strings_created++;
  return result;
}

void string_resize(string_t* string, size_t capacity) {
  if (capacity <= string->capacity) {
    return;
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
void string_dispose(string_t* string) {
  if (!string) {
    return;
  }

  free(string->pointer);
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

void string_dispose_n(string_t* strings[], size_t length) {
  while (length) {
    string_dispose(strings[length - 1]);
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

  if (right_index == search_string->length) {
    return 1;
  }

  return 0;
}

string_t* string_concat(string_t* left_string, string_t* right_string) {
  string_t* result = string_copy(left_string);
  if (result->capacity < result->length + right_string->length + 1) {
    string_resize(result, result->capacity + right_string->length + 1);
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
        "Capacity: %i\n"
        "Length: %i\n"
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
    string_dispose(prev_string);
    index++;
  }
  return result_string;
}

int main(int argc, char* argv[]) {
  size_t arg_index = 1;
  string_t* debug_search_string = string_create("--debug", 8);
  while (arg_index < argc) {
    string_t* arg_string = string_create(
        argv[arg_index], char_pointer_length(argv[arg_index], 128) + 1);
    if (string_includes(arg_string, debug_search_string)) {
      debug_mode = 1;
    }
    arg_index++;
    string_dispose(arg_string);
  }
  string_dispose(debug_search_string);

  string_t* strings[] = {
      string_create("My ", 64),      string_create("strings ", 9),
      string_create("are ", 5),      string_create("so ", 5),
      string_create("strong!\n", 9),
  };
  string_t* concated_string =
      string_concat_n(strings, sizeof(strings) / sizeof(string_t*));
  string_dispose(concated_string);
  string_dispose_n(strings, sizeof(strings) / sizeof(string_t*));
  return 0;
}
