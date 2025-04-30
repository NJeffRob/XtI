#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define VALID_FILE_PATH "src/program_test_files/h2.xyz"

int run_command(const char *cmd) {
  int status = system(cmd);
  return WEXITSTATUS(status);
}

START_TEST(test_valid_input_combo) {
  char command[512];
  snprintf(command, sizeof(command), "./bin/xti -sio orca freq %s 2>/dev/null",
           VALID_FILE_PATH);
  int exit_code = run_command(command);
  ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
                exit_code);
}
END_TEST

START_TEST(test_invalid_program) {
  char command[512];
  snprintf(command, sizeof(command),
           "./bin/xti -sio program freq %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(command);
  ck_assert_msg(exit_code != 0, "Expected failure for invalid program");
}
END_TEST

START_TEST(test_invalid_job_type) {
  char command[512];
  snprintf(command, sizeof(command),
           "./bin/xti -io orca job %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(command);
  ck_assert_msg(exit_code != 0, "Expected failure for invalid job type");
}
END_TEST

START_TEST(test_missing_file) {
  int exit_code =
      run_command("./bin/xti -so orca freq nonexistent.xyz 2>/dev/null");
  ck_assert_msg(exit_code != 0, "Expected failure due to missing file");
}
END_TEST

Suite *exec_suite(void) {
  Suite *s = suite_create("ExecTests");
  TCase *tc = tcase_create("Exec");

  tcase_add_test(tc, test_valid_input_combo);
  tcase_add_test(tc, test_invalid_program);
  tcase_add_test(tc, test_invalid_job_type);
  tcase_add_test(tc, test_missing_file);
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s = exec_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
