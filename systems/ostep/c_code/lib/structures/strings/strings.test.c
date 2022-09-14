#include "strings.h"

#include <string.h>

#include "../../../../../../minunit/minunit.h"

MU_TEST(string_equal_null_terminated_usual_test) {
  string_t* string = string_create("Hello", 5);

  mu_check(string_equal_null_terminated(string, "Hello", 5));
  string_dispose(&string);
}

MU_TEST_SUITE(string_equal_null_terminated_test_suite) {
  MU_RUN_TEST(string_equal_null_terminated_usual_test);
}

MU_TEST(string_create_empty_test) {
  string_t* string = string_create("", 0);

  mu_check(string != 0);
  mu_check(string->data != 0);
  mu_check(string->length == 0);
  mu_check(string->capacity == 0);
  string_dispose(&string);
}

MU_TEST(string_create_usual_test) {
  string_t* string = string_create("Hello world!", 12);

  mu_check(string != 0);
  mu_check(string->data != 0);
  mu_check(string->length == 12);
  mu_check(string->capacity == 12);
  string_dispose(&string);
}

MU_TEST(string_create_not_mutating_pointer_test) {
  char* pointer = "Hello";
  string_t* string = string_create(pointer, 5);

  mu_check(string->data != pointer);
  string_dispose(&string);
}

MU_TEST(string_create_less_capacity_test) {
  string_t* string = string_create("Hello world!", 5);

  mu_check(string != 0);
  mu_check(string->data != 0);
  mu_check(string->length == 5);
  mu_check(string->capacity == 5);
  mu_check(string_equal_null_terminated(string, "Hello", 5));
  string_dispose(&string);
}

MU_TEST_SUITE(string_create_test_suite) {
  MU_RUN_TEST(string_create_empty_test);
  MU_RUN_TEST(string_create_usual_test);
  MU_RUN_SUITE(string_create_not_mutating_pointer_test);
  MU_RUN_TEST(string_create_less_capacity_test);
}

MU_TEST(string_dispose_test) {
  string_t* string = string_create("Hello", 5);
  string_dispose(&string);

  mu_check(!string);
}

MU_TEST(string_dispose_n_test) {
  string_t* strings[2] = {string_create("Hello ", 6),
                          string_create("world!", 6)};

  string_dispose_n(strings, 2);
  mu_check(!strings[0]);
  mu_check(!strings[1]);
}

MU_TEST_SUITE(string_dispose_test_suite) {
  MU_RUN_TEST(string_dispose_test);
  MU_RUN_TEST(string_dispose_n_test);
}

MU_TEST(string_resize_bigger_test) {
  string_t* string = string_create("Hello", 5);
  string = string_resize(string, 12);

  mu_check(string_equal_null_terminated(string, "Hello", 5));
  mu_check(string->capacity == 12);
  mu_check(string->length == 5);
  string_dispose(&string);
}

MU_TEST(string_resize_smaller_test) {
  string_t* string = string_create("Hello world!", 12);
  string = string_resize(string, 4);

  mu_check(string_equal_null_terminated(string, "Hell", 4));
  mu_check(string->capacity == 4);
  mu_check(string->length == 4);
  string_dispose(&string);
}

MU_TEST(string_resize_same_test) {
  string_t* string = string_create("Hello", 5);
  string_t* new_string = string_resize(string, 5);

  mu_check(new_string == 0);
  string_dispose(&string);
}

MU_TEST_SUITE(string_resize_test_suite) {
  MU_RUN_TEST(string_resize_bigger_test);
  MU_RUN_TEST(string_resize_smaller_test);
  MU_RUN_TEST(string_resize_same_test);
}

MU_TEST(string_includes_not_included_test) {
  string_t* string = string_create("Hello", 5);
  string_t* search_string = string_create("world", 5);
  char is_included = string_includes(string, search_string);

  mu_check(!is_included);
  string_dispose(&string);
  string_dispose(&search_string);
}

MU_TEST(string_includes_included_test) {
  string_t* string = string_create("Hello", 5);
  string_t* search_string = string_create("Hell", 4);
  char is_included = string_includes(string, search_string);

  mu_check(is_included);
  string_dispose(&string);
  string_dispose(&search_string);
}

MU_TEST(string_includes_no_mutation_test) {
  string_t* string = string_create("Hello", 5);
  char* string_char_data = string->data;
  string_t* search_string = string_create("Hell", 4);
  char* search_string_data = search_string->data;
  string_includes(string, search_string);

  mu_check(string->data == string_char_data);
  mu_check(search_string->data == search_string_data);
  string_dispose(&string);
  string_dispose(&search_string);
}

MU_TEST_SUITE(string_includes_test_suite) {
  MU_RUN_TEST(string_includes_not_included_test);
  MU_RUN_TEST(string_includes_included_test);
  MU_RUN_TEST(string_includes_no_mutation_test);
}

MU_TEST(string_concat_usual_test) {
  string_t* left_string = string_create("Hello ", 6);
  string_t* right_string = string_create("world!", 6);
  string_t* hello_world_string = string_concat(left_string, right_string);

  mu_check(
      string_equal_null_terminated(hello_world_string, "Hello world!", 12));
  mu_check(hello_world_string->length == 12);
  mu_check(hello_world_string->capacity == 12);
  string_dispose(&left_string);
  string_dispose(&right_string);
  string_dispose(&hello_world_string);
}

MU_TEST(string_concat_left_empty_test) {
  string_t* left_string = string_create("", 0);
  string_t* right_string = string_create("right", 5);
  string_t* result_string = string_concat(left_string, right_string);

  mu_check(string_equal(result_string, right_string));
  mu_check(result_string->capacity == 5);
  mu_check(result_string->length == 5);
  string_dispose(&left_string);
  string_dispose(&right_string);
  string_dispose(&result_string);
}

MU_TEST(string_concat_right_empty_test) {
  string_t* left_string = string_create("left", 4);
  string_t* right_string = string_create("", 0);
  string_t* result_string = string_concat(left_string, right_string);

  mu_check(string_equal(result_string, left_string));
  mu_check(result_string->capacity == 4);
  mu_check(result_string->length == 4);

  string_dispose(&left_string);
  string_dispose(&right_string);
  string_dispose(&result_string);
}

MU_TEST(string_concat_both_empty_test) {
  string_t* left_string = string_create("", 0);
  string_t* right_string = string_create("", 0);
  string_t* result_string = string_concat(left_string, right_string);

  mu_check(string_equal(result_string, left_string));
  mu_check(string_equal(result_string, right_string));
  mu_check(result_string->capacity == 0);
  mu_check(result_string->length == 0);

  string_dispose(&left_string);
  string_dispose(&right_string);
  string_dispose(&result_string);
}

MU_TEST(string_concat_no_mutation_test) {
  string_t* left_string = string_create("Hello", 5);
  char* left_string_data = left_string->data;
  string_t* right_string = string_create("world!", 6);
  char* right_string_data = right_string->data;
  string_concat(left_string, right_string);

  mu_check(left_string->length == 5);
  mu_check(left_string->capacity == 5);
  mu_check(left_string->data == left_string_data);

  mu_check(right_string->length == 6);
  mu_check(right_string->capacity == 6);
  mu_check(right_string->data == right_string_data);

  string_dispose(&left_string);
  string_dispose(&right_string);
}

MU_TEST(string_concat_n_usual_test) {
  string_t* strings[] = {string_create("Hello", 5), string_create(" ", 1),
                         string_create("evil", 4), string_create(" ", 1),
                         string_create("world!", 6)};
  string_t* result_string =
      string_concat_n(strings, sizeof(strings) / sizeof(string_t*));

  mu_check(
      string_equal_null_terminated(result_string, "Hello evil world!", 17));
  mu_check(result_string->length == 17);
  mu_check(result_string->capacity == 17);

  string_dispose_n(strings, sizeof(strings) / sizeof(string_t*));
  string_dispose(&result_string);
}

MU_TEST(string_concat_n_empty_test) {
  string_t* strings[0] = {};
  string_t* result_string = string_concat_n(strings, 0);

  mu_check(string_equal_null_terminated(result_string, "", 0));
  mu_check(result_string->length == 0);
  mu_check(result_string->capacity == 0);

  string_dispose(&result_string);
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
  string_t* string = string_create("   hello      world!     ", 25);
  string_once_spaces(string);

  mu_check(string_equal_null_terminated(string, " hello world! ", 14));
  mu_check(string->length == 14);
  mu_check(string->capacity == 25);

  string_dispose(&string);
}

MU_TEST(string_once_spaces_only_spaces_test) {
  string_t* string = string_create("         ", 9);
  string_once_spaces(string);

  mu_check(string_equal_null_terminated(string, " ", 1));
  mu_check(string->length == 1);
  mu_check(string->capacity == 9);

  string_dispose(&string);
}

MU_TEST(string_once_spaces_no_spaces_test) {
  string_t* string = string_create("helloworld!", 11);
  string_once_spaces(string);

  mu_check(string_equal_null_terminated(string, "helloworld!", 11));
  mu_check(string->length == 11);
  mu_check(string->capacity == 11);

  string_dispose(&string);
}

MU_TEST_SUITE(string_once_spaces_test_suite) {
  MU_RUN_TEST(string_once_spaces_usual_test);
  MU_RUN_TEST(string_once_spaces_only_spaces_test);
  MU_RUN_TEST(string_once_spaces_no_spaces_test);
}

MU_TEST(string_split_char_char_exists_test) {
  string_t* string = string_create("One,two", 7);
  size_t buffer_length = 0;
  string_t** buffer = string_split_char(string, ',', &buffer_length);

  mu_check(buffer_length == 2);
  mu_check(string_equal_null_terminated(buffer[0], "One", 3));
  mu_check(buffer[0]->length == 3);
  mu_check(string_equal_null_terminated(buffer[1], "two", 3));
  mu_check(buffer[1]->length == 3);

  string_dispose(&string);
  string_dispose_n(buffer, buffer_length);
}

MU_TEST(string_split_char_char_does_not_exist_test) {
  string_t* string = string_create("Whole string", 12);
  size_t buffer_length = 0;
  string_t** buffer = string_split_char(string, ',', &buffer_length);

  mu_check(buffer_length == 1);
  mu_check(string_equal_null_terminated(buffer[0], "Whole string", 12));
  mu_check(buffer[0]->length == 12);

  string_dispose(&string);
  string_dispose_n(buffer, buffer_length);
}

MU_TEST(string_split_char_only_char_test) {
  string_t* string = string_create(",", 1);
  size_t buffer_length = 0;
  string_t** buffer = string_split_char(string, ',', &buffer_length);

  mu_check(buffer_length == 2);
  mu_check(string_equal_null_terminated(buffer[0], "", 0));
  mu_check(buffer[0]->length == 0);
  mu_check(string_equal_null_terminated(buffer[1], "", 0));
  mu_check(buffer[1]->length == 0);

  string_dispose(&string);
  string_dispose_n(buffer, buffer_length);
}

MU_TEST_SUITE(string_split_char_test_suite) {
  MU_RUN_TEST(string_split_char_char_exists_test);
  MU_RUN_TEST(string_split_char_char_does_not_exist_test);
  MU_RUN_TEST(string_split_char_only_char_test);
}

MU_TEST(string_trim_start_spaces_exist_test) {
  string_t* string = string_create("    Hello!", 10);
  string_trim_start(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_start_spaces_do_not_exist_test) {
  string_t* string = string_create("Hello!", 6);
  string_trim_start(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_start_end_spaces_test) {
  string_t* string = string_create("Hello!        ", 14);
  string_trim_start(string);

  mu_check(string_equal_null_terminated(string, "Hello!        ", 14));
  mu_check(string->length == 14);

  string_dispose(&string);
}

MU_TEST_SUITE(string_trim_start_test_suite) {
  MU_RUN_TEST(string_trim_start_spaces_exist_test);
  MU_RUN_TEST(string_trim_start_spaces_do_not_exist_test);
  MU_RUN_TEST(string_trim_start_end_spaces_test);
}

MU_TEST(string_trim_end_spaces_exist_test) {
  string_t* string = string_create("Hello!        ", 14);
  string_trim_end(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_end_spaces_do_not_exist_test) {
  string_t* string = string_create("Hello!", 6);
  string_trim_end(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_end_start_spaces_test) {
  string_t* string = string_create("   Hello!", 9);
  string_trim_end(string);

  mu_check(string_equal_null_terminated(string, "   Hello!", 9));
  mu_check(string->length == 9);

  string_dispose(&string);
}

MU_TEST_SUITE(string_trim_end_test_suite) {
  MU_RUN_TEST(string_trim_end_spaces_exist_test);
  MU_RUN_TEST(string_trim_end_spaces_do_not_exist_test);
  MU_RUN_TEST(string_trim_end_start_spaces_test);
}

MU_TEST(string_trim_start_spaces_only_test) {
  string_t* string = string_create("   Hello!", 9);
  string_trim(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_end_spaces_only_test) {
  string_t* string = string_create("Hello!   ", 9);
  string_trim(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_both_ends_spaces_test) {
  string_t* string = string_create("    Hello!   ", 13);
  string_trim(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST(string_trim_no_spaces_test) {
  string_t* string = string_create("Hello!", 6);
  string_trim(string);

  mu_check(string_equal_null_terminated(string, "Hello!", 6));
  mu_check(string->length == 6);

  string_dispose(&string);
}

MU_TEST_SUITE(string_trim_test_suite) {
  MU_RUN_TEST(string_trim_start_spaces_only_test);
  MU_RUN_TEST(string_trim_end_spaces_only_test);
  MU_RUN_TEST(string_trim_both_ends_spaces_test);
  MU_RUN_TEST(string_trim_no_spaces_test);
}

MU_TEST(string_join_char_not_empty_buffer_test) {
  string_t* strings[2] = {string_create("Hello", 5),
                          string_create("World!", 6)};
  string_t* string = string_join_char(strings, 2, ' ');

  mu_check(string_equal_null_terminated(string, "Hello World!", 12));
  mu_check(string->length == 12);

  string_dispose(&string);
  string_dispose_n(strings, 2);
}

MU_TEST(string_join_char_empty_buffer_test) {
  string_t* strings[0] = {};
  string_t* string = string_join_char(strings, 0, ' ');

  mu_check(string_equal_null_terminated(string, "", 0));
  mu_check(string->length == 0);

  string_dispose(&string);
}

MU_TEST_SUITE(string_join_char_test_suite) {
  MU_RUN_TEST(string_join_char_not_empty_buffer_test);
  MU_RUN_TEST(string_join_char_empty_buffer_test);
}

MU_TEST(string_starts_with_char_starts_test) {
  string_t* string = string_create("Hello", 5);
  char starts_with_h = string_starts_with_char(string, 'H');

  mu_check(starts_with_h);

  string_dispose(&string);
}

MU_TEST(string_starts_with_char_does_not_start_test) {
  string_t* string = string_create("Hello", 5);
  char starts_with_d = string_starts_with_char(string, 'D');

  mu_check(!starts_with_d);

  string_dispose(&string);
}

MU_TEST_SUITE(string_starts_with_char_test_suite) {
  MU_RUN_TEST(string_starts_with_char_starts_test);
  MU_RUN_TEST(string_starts_with_char_does_not_start_test);
}

MU_TEST(string_to_null_terminated_empty_test) {
  string_t* string = string_create("", 0);
  char* null_terminated = string_to_null_terminated(string);

  mu_check(null_terminated != 0);
  mu_check(null_terminated[0] == 0);

  free(null_terminated);
  string_dispose(&string);
}

MU_TEST(string_to_null_terminated_existing_test) {
  string_t* string = string_create("Hello", 5);
  char* null_terminated = string_to_null_terminated(string);

  mu_check(null_terminated != 0);
  mu_check(!strcmp(null_terminated, "Hello"));

  free(null_terminated);
  string_dispose(&string);
}

MU_TEST_SUITE(string_to_null_terminated_test_suite) {
  MU_RUN_TEST(string_to_null_terminated_empty_test);
  MU_RUN_TEST(string_to_null_terminated_existing_test);
}

int main(int argc, char* argv[]) {
  MU_RUN_SUITE(string_equal_null_terminated_test_suite);
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
  MU_RUN_SUITE(string_starts_with_char_test_suite);
  MU_RUN_SUITE(string_to_null_terminated_test_suite);
  MU_REPORT();
  return MU_EXIT_CODE;
}
