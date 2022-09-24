#include "../../../../../../minunit/minunit.h"
#include "array.h"

MU_TEST(array_create_test) {
  array_t* array = 0;
  enum ss_status status = array_create(10, &array);

  mu_check(status == SS_OK);
  mu_check(array != 0);
}

MU_TEST(array_create_less_one_capacity_test) {
  array_t* array = 0;
  enum ss_status status = array_create(0, &array);

  mu_check(status == SS_ERR_INVALID_CAPACITY);
  mu_check(array == 0);
}

MU_TEST(array_create_allocate_error_test) {
  array_t* array = 0;
  enum ss_status status = array_create(9999999999999999, &array);

  mu_check(status == SS_ERR_ALLOC);
  mu_check(array == 0);
}

MU_TEST_SUITE(array_create_test_suite) {
  MU_RUN_TEST(array_create_test);
  MU_RUN_TEST(array_create_less_one_capacity_test);
  MU_RUN_TEST(array_create_allocate_error_test);
}

MU_TEST(array_destroy_test) {
  array_t* array = 0;
  array_create(10, &array);

  array_destroy(&array);

  mu_check(array == 0);
}

MU_TEST_SUITE(array_destroy_test_suite) {
  MU_RUN_TEST(array_destroy_test);
}

MU_TEST(array_add_at_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  enum ss_status status = array_add_at(array, 0, &i);
  void* out = 0;
  array_get_at(array, 0, &out);

  mu_check(status == SS_OK);
  mu_check(array_length(array) == 1);
  mu_check(*(size_t*)out == 10);
}

MU_TEST(array_add_at_multiple_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  size_t j = 20;
  size_t k = 30;
  array_add_at(array, 0, &i);
  array_add_at(array, 1, &j);
  array_add_at(array, 2, &k);

  void* out_0 = 0;
  void* out_1 = 0;
  void* out_2 = 0;

  array_get_at(array, 0, &out_0);
  array_get_at(array, 1, &out_1);
  array_get_at(array, 2, &out_2);

  mu_check(*(size_t*)out_0 == 10);
  mu_check(*(size_t*)out_1 == 20);
  mu_check(*(size_t*)out_2 == 30);
  mu_check(array_length(array) == 3);
}

MU_TEST(array_add_at_shift_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  size_t j = 20;
  size_t k = 30;
  array_add_at(array, 0, &i);
  array_add_at(array, 0, &j);
  array_add_at(array, 0, &k);

  void* out_0 = 0;
  void* out_1 = 0;
  void* out_2 = 0;

  array_get_at(array, 0, &out_0);
  array_get_at(array, 1, &out_1);
  array_get_at(array, 2, &out_2);

  mu_check(*(size_t*)out_0 == 30);
  mu_check(*(size_t*)out_1 == 20);
  mu_check(*(size_t*)out_2 == 10);
}

MU_TEST(array_add_at_max_capacity_test) {
  array_t* array = 0;
  array_create(2, &array);

  size_t i = 10;
  array_add_at(array, 0, &i);
  enum ss_status ok_status = array_add_at(array, 0, &i);
  enum ss_status max_capacity_status = array_add_at(array, 0, &i);

  mu_check(ok_status == SS_OK);
  mu_check(max_capacity_status == SS_ERR_MAX_CAPACITY);
  mu_check(array_length(array) == 2);
}

MU_TEST_SUITE(array_add_at_test_suite) {
  MU_RUN_TEST(array_add_at_test);
  MU_RUN_TEST(array_add_at_multiple_test);
  MU_RUN_TEST(array_add_at_shift_test);
  MU_RUN_TEST(array_add_at_max_capacity_test);
}

MU_TEST(array_add_last_zero_length_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  enum ss_status status = array_add_last(array, &i);
  void* out = 0;
  array_get_at(array, 0, &out);

  mu_check(status == SS_OK);
  mu_check(array_length(array) == 1);
  mu_check(*(size_t*)out == 10);
}

MU_TEST(array_add_last_non_zero_length_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  array_add_last(array, &i);
  array_add_last(array, &i);
  array_add_last(array, &i);

  mu_check(array_length(array) == 3);
}

MU_TEST_SUITE(array_add_last_test_suite) {
  MU_RUN_TEST(array_add_last_zero_length_test);
  MU_RUN_TEST(array_add_last_non_zero_length_test);
}

MU_TEST(array_remove_at_zero_length_test) {
  array_t* array = 0;
  array_create(10, &array);

  void* out = 0;
  enum ss_status status = array_remove_at(array, 0, &out);

  mu_check(status == SS_ERR_OUT_OF_RANGE);
}

MU_TEST(array_remove_at_non_zero_length_test) {
  array_t* array = 0;
  array_create(10, &array);
  size_t i = 10;
  array_add_last(array, &i);

  void* out = 0;
  enum ss_status status = array_remove_at(array, 0, &out);

  mu_check(status == SS_OK);
  mu_check(*(size_t*)out == 10);
  mu_check(array_length(array) == 0);
}

MU_TEST(array_remove_at_shift_test) {
  array_t* array = 0;
  array_create(10, &array);
  size_t i = 10;
  size_t j = 20;
  size_t k = 30;
  array_add_last(array, &i);
  array_add_last(array, &j);
  array_add_last(array, &k);

  size_t* remove_out_1 = 0;
  array_remove_at(array, 1, (void**)&remove_out_1);

  size_t* get_out_0 = 0;
  size_t* get_out_1 = 0;
  array_get_at(array, 0, (void**)&get_out_0);
  array_get_at(array, 1, (void**)&get_out_1);

  mu_check(*remove_out_1 == 20);
  mu_check(*get_out_0 == 10);
  mu_check(*get_out_1 == 30);
}

MU_TEST_SUITE(array_remove_at_test_suite) {
  MU_RUN_TEST(array_remove_at_zero_length_test);
  MU_RUN_TEST(array_remove_at_non_zero_length_test);
  MU_RUN_TEST(array_remove_at_shift_test);
}

MU_TEST(array_remove_last_test) {
  array_t* array = 0;
  array_create(10, &array);
  size_t i = 10;
  array_add_last(array, &i);

  size_t* out = 0;
  enum ss_status status = array_remove_last(array, (void**)&out);

  mu_check(array_length(array) == 0);
  mu_check(*out == 10);
  mu_check(status == SS_OK);
}

MU_TEST_SUITE(array_remove_last_test_suite) {
  MU_RUN_TEST(array_remove_last_test);
}

MU_TEST(array_replace_at_out_of_range_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  enum ss_status status = array_replace_at(array, 0, &i);
  mu_check(status == SS_ERR_OUT_OF_RANGE);

  array_add_last(array, &i);
  array_add_last(array, &i);

  size_t j = 20;
  status = array_replace_at(array, 0, &j);
  mu_check(status == SS_OK);

  status = array_replace_at(array, 8, &j);
  mu_check(status == SS_ERR_OUT_OF_RANGE);
}

MU_TEST(array_replace_at_ok_test) {
  array_t* array = 0;
  array_create(10, &array);

  size_t i = 10;
  array_add_last(array, &i);
  enum ss_status status = array_replace_at(array, 0, &i);
  mu_check(status == SS_OK);
}

MU_TEST_SUITE(array_replace_at_test_suite) {
  MU_RUN_TEST(array_replace_at_out_of_range_test);
  MU_RUN_TEST(array_replace_at_ok_test);
}

int main(int argc, char* argv[]) {
  MU_RUN_SUITE(array_create_test_suite);
  MU_RUN_SUITE(array_destroy_test_suite);
  MU_RUN_SUITE(array_add_at_test_suite);
  MU_RUN_SUITE(array_add_last_test_suite);
  MU_RUN_SUITE(array_remove_at_test_suite);
  MU_RUN_SUITE(array_remove_last_test_suite);
  MU_RUN_SUITE(array_replace_at_test_suite);

  MU_REPORT();
  return MU_EXIT_CODE;
}

