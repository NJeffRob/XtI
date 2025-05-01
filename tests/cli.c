#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define VALID_FILE_PATH "src/program_test_files/h2.xyz"

char commandLine[256];

int run_command(const char *cmd) {
  int status = system(cmd);
  return WEXITSTATUS(status);
}

START_TEST(test_valid_input) {
  snprintf(commandLine, sizeof(commandLine),
           "./bin/xti -sij orca freq %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
                exit_code);
}
END_TEST

START_TEST(test_invalid_simultaneous_in_out) {
  snprintf(commandLine, sizeof(commandLine),
           "./bin/xti -ioj orca freq %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0,
                "Expected failure for simultaneous input/output call");
}
END_TEST

START_TEST(test_invalid_option_j_without_i) {
  snprintf(commandLine, sizeof(commandLine),
           "./bin/xti -soj orca freq %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0,
                "Expected failure for using option 'j' without option 'i'");
}
END_TEST

START_TEST(test_invalid_program) {
  snprintf(commandLine, sizeof(commandLine),
           "./bin/xti -so program freq %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0, "Expected failure for invalid program");
}
END_TEST

START_TEST(test_invalid_job) {
  snprintf(commandLine, sizeof(commandLine),
           "./bin/xti -ij orca job %s 2>/dev/null", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0, "Expected failure for invalid job type");
}
END_TEST

START_TEST(test_missing_file) {
  int exit_code =
      run_command("./bin/xti -is orca freq nonexistent.xyz 2>/dev/null");
  ck_assert_msg(exit_code != 0, "Expected failure due to missing file");
}
END_TEST

Suite *exec_suite(void) {
  Suite *s = suite_create("ExecTests");
  TCase *tc = tcase_create("Exec");

  tcase_add_test(tc, test_valid_input);
  tcase_add_test(tc, test_invalid_simultaneous_in_out);
  tcase_add_test(tc, test_invalid_option_j_without_i);
  tcase_add_test(tc, test_invalid_program);
  tcase_add_test(tc, test_invalid_job);
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
