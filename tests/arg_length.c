#include "../include/util.h"
#include <check.h>

START_TEST(test_pass_valid_lengths) {
  ck_assert(is_valid_length("test", 1, 5));
  ck_assert(is_valid_length("hi", 2, 2));
}
END_TEST

START_TEST(test_fail_invalid_lengths) {
  ck_assert(!is_valid_length("", 1, 5));
  ck_assert(!is_valid_length("invalidstring", 1, 5));
  ck_assert(!is_valid_length(NULL, 1, 5));
}
END_TEST

Suite *arg_length_suite(void) {
  Suite *s = suite_create("arg_length");

  TCase *tc_pass = tcase_create("Pass");
  TCase *tc_fail = tcase_create("Fail");

  tcase_add_test(tc_pass, test_pass_valid_lengths);

  tcase_add_test(tc_fail, test_fail_invalid_lengths);

  suite_add_tcase(s, tc_pass);
  suite_add_tcase(s, tc_fail);

  return s;
}

int main(void) {
  int failed;
  Suite *s = arg_length_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}
