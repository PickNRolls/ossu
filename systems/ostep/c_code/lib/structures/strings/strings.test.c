#include "strings.h"

#include <string.h>

#include "../../../../../../minunit/minunit.h"

MU_TEST(string_create_empty_test) {
  string_t* string = string_create("", 1);

  mu_check(string != 0);
  mu_check(string->pointer != 0);
  mu_check(string->length == 0);
  mu_check(string->capacity == 1);
}

MU_TEST(string_create_usual_test) {
  string_t* string = string_create("Hello world!", 13);

  mu_check(string != 0);
  mu_check(string->pointer != 0);
  mu_check(string->length == 12);
  mu_check(string->capacity == 13);
}

MU_TEST(string_create_not_mutating_pointer_test) {
  char* pointer = "Hello";
  string_t* string = string_create(pointer, 6);

  mu_check(string->pointer != pointer);
  mu_check(!strcmp(pointer, "Hello"));
}

MU_TEST(string_create_less_capacity_test) {
  string_t* string = string_create("Hello world!", 6);

  mu_check(string != 0);
  mu_check(string->pointer != 0);
  mu_check(!strcmp(string->pointer, "Hello"));
  mu_check(string->length == 5);
  mu_check(string->capacity == 6);
}

MU_TEST_SUITE(string_create_test_suite) {
  MU_RUN_TEST(string_create_empty_test);
  MU_RUN_TEST(string_create_usual_test);
  MU_RUN_SUITE(string_create_not_mutating_pointer_test);
  MU_RUN_TEST(string_create_less_capacity_test);
}

MU_TEST(string_dispose_test) {
  string_t* string = string_create("Hello", 6);
  string_dispose(&string);

  mu_check(!string);
}

MU_TEST(string_dispose_n_test) {
  string_t* strings[2] = {string_create("Hello ", 7),
                          string_create("world!", 7)};

  string_dispose_n(strings, 2);
  mu_check(!strings[0]);
  mu_check(!strings[1]);
}

MU_TEST_SUITE(string_dispose_test_suite) {
  MU_RUN_TEST(string_dispose_test);
  MU_RUN_TEST(string_dispose_n_test);
}

MU_TEST(string_resize_bigger_test) {
  string_t* string = string_create("Hello", 6);
  string_resize(string, 12);

  mu_check(!strcmp(string->pointer, "Hello"));
  mu_check(string->capacity == 12);
  mu_check(string->length == 5);
}

MU_TEST(string_resize_smaller_test) {
  string_t* string = string_create("Hello", 6);
  string_resize(string, 4);

  mu_check(!strcmp(string->pointer, "Hel"));
  mu_check(string->capacity == 4);
  mu_check(string->length == 3);
}

MU_TEST(string_resize_same_test) {
  string_t* string = string_create("Hello", 6);
  string_resize(string, 6);

  mu_check(!strcmp(string->pointer, "Hello"));
  mu_check(string->capacity == 6);
  mu_check(string->length == 5);
}

MU_TEST_SUITE(string_resize_test_suite) {
  MU_RUN_TEST(string_resize_bigger_test);
  MU_RUN_TEST(string_resize_smaller_test);
  MU_RUN_TEST(string_resize_same_test);
}

MU_TEST(string_includes_not_included_test) {
  string_t* string = string_create("Hello", 6);
  string_t* search_string = string_create("world", 6);
  char is_included = string_includes(string, search_string);

  mu_check(!is_included);
}

MU_TEST(string_includes_included_test) {
  string_t* string = string_create("Hello", 6);
  string_t* search_string = string_create("Hell", 5);
  char is_included = string_includes(string, search_string);

  mu_check(is_included);
}

MU_TEST(string_includes_no_mutation_test) {
  string_t* string = string_create("Hello", 6);
  char* string_char_pointer = string->pointer;
  string_t* search_string = string_create("Hell", 5);
  char* search_string_pointer = search_string->pointer;
  string_includes(string, search_string);

  mu_check(string->pointer == string_char_pointer);
  mu_check(search_string->pointer == search_string_pointer);
}

MU_TEST_SUITE(string_includes_test_suite) {
  MU_RUN_TEST(string_includes_not_included_test);
  MU_RUN_TEST(string_includes_included_test);
  MU_RUN_TEST(string_includes_no_mutation_test);
}

MU_TEST(string_concat_usual_test) {
  string_t* left_string = string_create("Hello ", 7);
  string_t* right_string = string_create("world!", 7);
  string_t* hello_world_string = string_concat(left_string, right_string);

  mu_check(!strcmp(hello_world_string->pointer, "Hello world!"));
  mu_check(hello_world_string->length == 12);
  mu_check(hello_world_string->capacity == 13);
}

MU_TEST(string_concat_left_empty_test) {
  string_t* left_string = string_create("", 1);
  string_t* right_string = string_create("right", 6);
  string_t* result_string = string_concat(left_string, right_string);

  mu_check(!strcmp(result_string->pointer, "right"));
  mu_check(result_string->capacity == 6);
  mu_check(result_string->length == 5);
}

MU_TEST(string_concat_right_empty_test) {
  string_t* left_string = string_create("left", 5);
  string_t* right_string = string_create("", 1);
  string_t* result_string = string_concat(left_string, right_string);

  mu_check(!strcmp(result_string->pointer, "left"));
  mu_check(result_string->capacity == 5);
  mu_check(result_string->length == 4);
}

MU_TEST(string_concat_both_empty_test) {
  string_t* left_string = string_create("", 1);
  string_t* right_string = string_create("", 1);
  string_t* result_string = string_concat(left_string, right_string);

  mu_check(!strcmp(result_string->pointer, ""));
  mu_check(result_string->capacity == 1);
  mu_check(result_string->length == 0);
}

MU_TEST(string_concat_no_mutation_test) {
  string_t* left_string = string_create("Hello", 6);
  char* left_string_pointer = left_string->pointer;
  string_t* right_string = string_create("world!", 7);
  char* right_string_pointer = right_string->pointer;
  string_concat(left_string, right_string);

  mu_check(!strcmp(left_string->pointer, "Hello"));
  mu_check(left_string->pointer == left_string_pointer);
  mu_check(left_string->length == 5);
  mu_check(left_string->capacity == 6);

  mu_check(!strcmp(right_string->pointer, "world!"));
  mu_check(right_string->length == 6);
  mu_check(right_string->capacity == 7);
  mu_check(right_string->pointer == right_string_pointer);
}

MU_TEST(string_concat_n_usual_test) {
  string_t* strings[] = {string_create("Hello", 6), string_create(" ", 2),
                         string_create("evil", 5), string_create(" ", 2),
                         string_create("world!", 7)};
  string_t* result_string =
      string_concat_n(strings, sizeof(strings) / sizeof(string_t*));

  mu_check(!strcmp(result_string->pointer, "Hello evil world!"));
  mu_check(result_string->length == 17);
  mu_check(result_string->capacity == 18);
}

MU_TEST(string_concat_n_empty_test) {
  string_t* strings[0] = {};
  string_t* result_string = string_concat_n(strings, 0);

  mu_check(!strcmp(result_string->pointer, ""));
  mu_check(result_string->length == 0);
  mu_check(result_string->capacity == 1);
}

MU_TEST_SUITE(string_concat_test_suite) {
  MU_RUN_TEST(string_concat_usual_test);
  MU_RUN_TEST(string_concat_left_empty_test);
  MU_RUN_TEST(string_concat_right_empty_test);
  MU_RUN_TEST(string_concat_both_empty_test);
  MU_RUN_TEST(string_concat_no_mutation_test);

  MU_RUN_TEST(string_concat_n_usual_test);
  MU_RUN_TEST(string_concat_n_empty_test);
}

MU_TEST(string_once_spaces_usual_test) {
  string_t* string = string_create("   hello      world!     ", 26);
  string_once_spaces(string);

  mu_check(!strcmp(string->pointer, " hello world! "));
  mu_check(string->length == 14);
  mu_check(string->capacity == 26);
}

MU_TEST(string_once_spaces_only_spaces_test) {
  string_t* string = string_create("         ", 10);
  string_once_spaces(string);

  mu_check(!strcmp(string->pointer, " "));
  mu_check(string->length == 1);
  mu_check(string->capacity == 10);
}

MU_TEST(string_once_spaces_no_spaces_test) {
  string_t* string = string_create("helloworld!", 12);
  string_once_spaces(string);

  mu_check(!strcmp(string->pointer, "helloworld!"));
  mu_check(string->length == 11);
  mu_check(string->capacity == 12);
}

MU_TEST_SUITE(string_once_spaces_test_suite) {
  MU_RUN_TEST(string_once_spaces_usual_test);
  MU_RUN_TEST(string_once_spaces_only_spaces_test);
  MU_RUN_TEST(string_once_spaces_no_spaces_test);
}

MU_TEST(string_split_char_char_exists_test) {
  string_t* string = string_create("One,two", 8);
  size_t buffer_length = 0;
  string_t** buffer = string_split_char(string, ',', &buffer_length);

  mu_check(buffer_length == 2);
  mu_check(!strcmp(buffer[0]->pointer, "One"));
  mu_check(buffer[0]->length == 3);
  mu_check(!strcmp(buffer[1]->pointer, "two"));
  mu_check(buffer[1]->length == 3);
}

MU_TEST(string_split_char_char_does_not_exist_test) {
  string_t* string = string_create("Whole string", 13);
  size_t buffer_length = 0;
  string_t** buffer = string_split_char(string, ',', &buffer_length);

  mu_check(buffer_length == 1);
  mu_check(!strcmp(buffer[0]->pointer, "Whole string"));
  mu_check(buffer[0]->length == 12);
}

MU_TEST(string_split_char_only_char_test) {
  string_t* string = string_create(",", 2);
  size_t buffer_length = 0;
  string_t** buffer = string_split_char(string, ',', &buffer_length);

  mu_check(buffer_length == 2);
  mu_check(!strcmp(buffer[0]->pointer, ""));
  mu_check(buffer[0]->length == 0);
  mu_check(!strcmp(buffer[1]->pointer, ""));
  mu_check(buffer[1]->length == 0);
}

MU_TEST_SUITE(string_split_char_test_suite) {
  MU_RUN_TEST(string_split_char_char_exists_test);
  MU_RUN_TEST(string_split_char_char_does_not_exist_test);
  MU_RUN_TEST(string_split_char_only_char_test);
}

MU_TEST(string_trim_start_spaces_exist_test) {
  string_t* string = string_create("    Hello!", 11);
  string_trim_start(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_start_spaces_do_not_exist_test) {
  string_t* string = string_create("Hello!", 7);
  string_trim_start(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_start_end_spaces_test) {
  string_t* string = string_create("Hello!        ", 15);
  string_trim_start(string);

  mu_check(!strcmp(string->pointer, "Hello!        "));
  mu_check(string->length == 14);
}

MU_TEST_SUITE(string_trim_start_test_suite) {
  MU_RUN_TEST(string_trim_start_spaces_exist_test);
  MU_RUN_TEST(string_trim_start_spaces_do_not_exist_test);
  MU_RUN_TEST(string_trim_start_end_spaces_test);
}

MU_TEST(string_trim_end_spaces_exist_test) {
  string_t* string = string_create("Hello!        ", 15);
  string_trim_end(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_end_spaces_do_not_exist_test) {
  string_t* string = string_create("Hello!", 7);
  string_trim_end(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_end_start_spaces_test) {
  string_t* string = string_create("   Hello!", 10);
  string_trim_end(string);

  mu_check(!strcmp(string->pointer, "   Hello!"));
  mu_check(string->length == 9);
}

MU_TEST_SUITE(string_trim_end_test_suite) {
  MU_RUN_TEST(string_trim_end_spaces_exist_test);
  MU_RUN_TEST(string_trim_end_spaces_do_not_exist_test);
  MU_RUN_TEST(string_trim_end_start_spaces_test);
}

MU_TEST(string_trim_start_spaces_only_test) {
  string_t* string = string_create("   Hello!", 10);
  string_trim(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_end_spaces_only_test) {
  string_t* string = string_create("Hello!   ", 10);
  string_trim(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_both_ends_spaces_test) {
  string_t* string = string_create("    Hello!   ", 14);
  string_trim(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST(string_trim_no_spaces_test) {
  string_t* string = string_create("Hello!", 7);
  string_trim(string);

  mu_check(!strcmp(string->pointer, "Hello!"));
  mu_check(string->length == 6);
}

MU_TEST_SUITE(string_trim_test_suite) {
  MU_RUN_TEST(string_trim_start_spaces_only_test);
  MU_RUN_TEST(string_trim_end_spaces_only_test);
  MU_RUN_TEST(string_trim_both_ends_spaces_test);
  MU_RUN_TEST(string_trim_no_spaces_test);
}

MU_TEST(string_join_char_not_empty_buffer_test) {
  string_t* strings[2] = {string_create("Hello", 6),
                          string_create("World!", 7)};
  string_t* string = string_join_char(strings, 2, ' ');

  mu_check(!strcmp(string->pointer, "Hello World!"));
  mu_check(string->length == 12);
}

MU_TEST(string_join_char_empty_buffer_test) {
  string_t* strings[0] = {};
  string_t* string = string_join_char(strings, 0, ' ');

  mu_check(!strcmp(string->pointer, " "));
  mu_check(string->length == 1);
}

MU_TEST_SUITE(string_join_char_test_suite) {
  MU_RUN_TEST(string_join_char_not_empty_buffer_test);
  MU_RUN_TEST(string_join_char_empty_buffer_test);
}

int main(int argc, char* argv[]) {
  MU_RUN_SUITE(string_create_test_suite);
  MU_RUN_SUITE(string_dispose_test_suite);
  MU_RUN_SUITE(string_resize_test_suite);
  MU_RUN_SUITE(string_includes_test_suite);
  MU_RUN_SUITE(string_concat_test_suite);
  MU_RUN_SUITE(string_once_spaces_test_suite);
  MU_RUN_SUITE(string_split_char_test_suite);
  MU_RUN_SUITE(string_trim_start_test_suite);
  MU_RUN_SUITE(string_trim_end_test_suite);
  MU_RUN_SUITE(string_trim_test_suite);
  MU_RUN_SUITE(string_join_char_test_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
