#include "../include/file_handler.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(test_pass_xyz) {
  ck_assert(file_xyz_extension("some_crazy_file.xyz"));
  ck_assert(file_xyz_extension("test.xyz"));
}
END_TEST

START_TEST(test_fail_xyz) {
  ck_assert(!file_xyz_extension("typo1.xy"));
  ck_assert(!file_xyz_extension("typo2.yz"));
  ck_assert(!file_xyz_extension("typo3.xz"));
  ck_assert(!file_xyz_extension("wrong.sh"));
}
END_TEST

Suite *file_handler_suite(void) {
  Suite *s = suite_create("file_handler");

  TCase *tc_pass = tcase_create("Pass");
  TCase *tc_fail = tcase_create("Fail");

  tcase_add_test(tc_pass, test_pass_xyz);

  tcase_add_test(tc_fail, test_fail_xyz);

  suite_add_tcase(s, tc_pass);
  suite_add_tcase(s, tc_fail);

  return s;
}

int main(void) {
  int failed = 0;
  Suite *s = file_handler_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}
