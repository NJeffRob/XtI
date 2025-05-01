#include "../include/util.h"
#include <check.h>

START_TEST(test_valid_lengths) {
  ck_assert(is_valid_length("test", 1, 5));
  ck_assert(is_valid_length("hi", 2, 2));
}
END_TEST

START_TEST(test_invalid_lengths) {
  ck_assert(!is_valid_length("", 1, 5));
  ck_assert(!is_valid_length("invalidstring", 1, 5));
  ck_assert(!is_valid_length(NULL, 1, 5));
}
END_TEST

Suite *utils_suite(void) {
  Suite *s = suite_create("Utils");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_valid_lengths);
  tcase_add_test(tc, test_invalid_lengths);
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  int failed;
  Suite *s = utils_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}
