#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/structures/strings.h"

char* paths[] = {};

void setup_paths(char* paths[], size_t length) {}

void exec_command(string_t* command, string_t** arguments,
                  size_t arguments_length) {
  string_t* arguments_string =
      string_join_char(arguments, arguments_length, ',');
  printf("Command: %s\nArguments: %s\n", command->pointer,
         arguments_string->pointer);
  string_dispose(arguments_string);
}

void exec_line(string_t* line) {
  size_t space_substrings_length = 0;
  string_t** space_substrings =
      string_split_char(line, ' ', &space_substrings_length);

  if (!space_substrings_length) {
    string_dispose_n(space_substrings, space_substrings_length);
    free(space_substrings);
    return;
  }

  string_t* command_string = space_substrings[0];
  string_t** argument_strings = space_substrings + 1;
  exec_command(command_string, argument_strings,
               space_substrings_length > 1 ? space_substrings_length - 1 : 0);
  string_dispose_n(space_substrings, space_substrings_length);
  free(space_substrings);
}

string_t* get_line_string() {
  char* line = 0;
  size_t l = 0;
  size_t length = getline(&line, &l, stdin);
  if (length == -1) {
    exit(1);
  }
  return string_create(line, length);  // eliminate '\n' character
}

void prepare_line_string(string_t* line) {
  string_trim(line);
  string_once_spaces(line);
  string_resize(line, line->length + 1);
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    fprintf(stderr, "error\n");
    exit(1);
  }

  while (1) {
    printf("msh$ ");
    string_t* line = get_line_string();
    prepare_line_string(line);
    exec_line(line);
    string_dispose(line);
  }

  return 0;
}
