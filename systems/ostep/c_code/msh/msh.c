#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../lib/structures/strings/strings.h"

int main(int argc, char* argv[]) {
  if (argc > 2) {
    fprintf(stderr, "error\n");
    exit(1);
  }

  string_t** paths = malloc(sizeof(string_t*));
  size_t paths_length = 1;
  if (!paths) {
    fprintf(stderr, "failed to allocate memory for paths\n");
    exit(1);
  }
  paths[0] = string_create("/bin/", 5);

  string_t* command_not_found_string =
      string_create(": command not found\n", 20);

  string_t* exit_command = string_create("exit", 4);
  string_t* bye_command = string_create("bye", 3);
  string_t* path_command = string_create("path", 4);
  string_t* cd_command = string_create("cd", 2);

  while (1) {
    printf("msh> ");
    char* line = 0;
    size_t l = 0;
    size_t length = getline(&line, &l, stdin);
    if (length == -1) {
      exit(1);
    }
    if (!length || length == 1) {
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
    size_t arguments_length = space_substrings_length - 1;
    string_t** argument_strings = space_substrings + 1;

    if (string_equal(command_string, exit_command) ||
        string_equal(command_string, bye_command)) {
      exit(0);
    } else if (string_equal(command_string, cd_command)) {
      if (arguments_length != 1) {
        fprintf(stderr, "usage: cd <path>\n");
      } else {
        char* chdir_string = string_to_null_terminated(argument_strings[0]);
        int result = chdir(chdir_string);
        if (result) {
          fprintf(stderr, "cd: failed to change directory\n");
        }
        free(chdir_string);
      }
    } else if (string_equal(command_string, path_command)) {
      string_dispose_n(paths, paths_length);
      paths_length = 0;
      paths = 0;

      if (!arguments_length) {
        free(paths);
      } else {
        paths = realloc(paths, sizeof(string_t*) * arguments_length);
        if (!paths) {
          fprintf(stderr, "failed to reallocate memory for paths\n");
	  exit(1);
        }
        paths_length = arguments_length;
        size_t index = 0;
        while (index < arguments_length) {
          string_t* argument = argument_strings[index];
          char need_append_slash = !string_ends_with_char(argument, '/');
          size_t path_capacity = argument->capacity;
          if (need_append_slash && path_capacity < argument->length + 1) {
            path_capacity++;
          }
          paths[index] = string_duplicate(argument, path_capacity);
          if (need_append_slash) {
            string_append_char(paths[index], '/');
          }
          index++;
        }
      }
    } else {
      if (!paths_length) {
        fprintf(
            stderr,
            "empty path, can't execute any program besides built-in ones\n");
      } else {
        size_t index = 0;
        int found_executable = 0;
        char* absolute_path = 0;
        while (index < paths_length && !found_executable) {
          string_t* concated = string_concat(paths[index], command_string);
          absolute_path = string_to_null_terminated(concated);
          string_dispose(&concated);
          found_executable = !access(absolute_path, X_OK);
          index++;
        }

        if (!found_executable) {
          string_t* message_string =
              string_concat(command_string, command_not_found_string);
          fprintf(stderr, "%.*s", (int)message_string->length,
                  message_string->data);
          string_dispose(&message_string);
        } else {
          pid_t pid = fork();
          if (pid == -1) {
            fprintf(stderr, "failed to fork command program\n");
          } else if (pid) {
            waitpid(pid, 0, 0);
          } else {
            char* argv[arguments_length + 2];
            argv[0] = absolute_path;
            size_t index = 0;
            while (index < arguments_length) {
              argv[index + 1] =
                  string_to_null_terminated(argument_strings[index]);
              index++;
            }
            argv[index + 1] = 0;

            if (execv(absolute_path, argv)) {
              fprintf(stderr, "failed to exec program\n");
            }
          }
        }
        free(absolute_path);
      }
    }

    string_dispose_n(space_substrings, space_substrings_length);
    free(space_substrings);
    string_dispose(&string);
  }

  return 0;
}
