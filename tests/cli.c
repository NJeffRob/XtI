#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define VALID_FILE_PATH "src/program_test_files/h2.xyz"

char commandLine[256];

void build_command(const char *args, const char *filepath) {
  snprintf(commandLine, sizeof(commandLine), "./bin/xti %s %s 2>/dev/null",
           args, filepath);
}

int run_command(const char *cmd) {
  int status = system(cmd);
  return WEXITSTATUS(status);
}

START_TEST(test_pass_input) {
  build_command("-sij orca freq", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
                exit_code);
}
END_TEST

START_TEST(test_fail_simultaneous_in_out) {
  build_command("-ioj orca freq", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0,
                "Expected failure for simultaneous input/output call");
}
END_TEST

START_TEST(test_fail_option_j_without_i) {
  build_command("-soj orca freq", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0,
                "Expected failure for using option 'j' without option 'i'");
}
END_TEST

START_TEST(test_fail_invalid_program) {
  build_command("-so program freq", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0, "Expected failure for invalid program");
}
END_TEST

START_TEST(test_fail_invalid_job) {
  build_command("-ij orca job", VALID_FILE_PATH);
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0, "Expected failure for invalid job type");
}
END_TEST

START_TEST(test_fail_missing_file) {
  build_command("-is orca freq", "missing.xyz");
  int exit_code = run_command(commandLine);
  ck_assert_msg(exit_code != 0, "Expected failure due to missing file");
}
END_TEST

Suite *cli_suite(void) {
  Suite *s = suite_create("cli");

  TCase *tc_pass = tcase_create("Pass");
  TCase *tc_fail = tcase_create("Fail");

  tcase_add_test(tc_pass, test_pass_input);

  tcase_add_test(tc_fail, test_fail_simultaneous_in_out);
  tcase_add_test(tc_fail, test_fail_option_j_without_i);
  tcase_add_test(tc_fail, test_fail_invalid_program);
  tcase_add_test(tc_fail, test_fail_invalid_job);
  tcase_add_test(tc_fail, test_fail_missing_file);

  suite_add_tcase(s, tc_pass);
  suite_add_tcase(s, tc_fail);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s = cli_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
