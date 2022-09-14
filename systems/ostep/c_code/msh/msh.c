#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/structures/strings/strings.h"

char* paths[] = {};

int main(int argc, char* argv[]) {
  if (argc > 2) {
    fprintf(stderr, "error\n");
    exit(1);
  }

  string_t* exit_command = string_create("exit", 4);
  string_t* bye_command = string_create("bye", 3);
  string_t* available_commands[] = {exit_command, bye_command};
  char do_exit = 0;

  while (1) {
    printf("msh> ");
    char* line = 0;
    size_t l = 0;
    size_t length = getline(&line, &l, stdin);
    if (length == -1) {
      exit(1);
    }
    if (!length) {
      continue;
    }

    string_t* string =
        string_create(line, length - 1);  // eliminate '\n' character
    string_trim(string);
    string_once_spaces(string);
    size_t space_substrings_length = 0;
    string_t** space_substrings =
        string_split_char(string, ' ', &space_substrings_length);

    string_t* command_string = space_substrings[0];
    if (string_equal(command_string, exit_command) ||
        string_equal(command_string, bye_command)) {
      do_exit = 1;
    }

    string_dispose_n(space_substrings, space_substrings_length);
    free(space_substrings);
    string_dispose(&string);
    if (do_exit) {
      goto prepare_to_exit;
    }
  }

prepare_to_exit:
  string_dispose_n(available_commands,
                   sizeof(available_commands) / sizeof(string_t*));
  return 0;
}
