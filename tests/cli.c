#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define VALID_INPUT_FILE  "src/program_test_files/h2.xyz"
#define VALID_OUTPUT_FILE "src/program_test_files/gaussian/h2o-opt.log"

char commandLine[256];

void build_command(const char *args, const char *filepath) {
	snprintf(commandLine, sizeof(commandLine), "./bin/xti %s %s 2>/dev/null",
			 args, filepath);
}

int run_command(const char *cmd) {
	int status = system(cmd);
	return WEXITSTATUS(status);
}

START_TEST(test_pass_help) {
	build_command("-h", "");
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_pass_upper_help) {
	build_command("-H", "");
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_pass_input) {
	build_command("-i orca freQ", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_pass_input_script) {
	build_command("-si orca freQ", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_pass_default_job) {
	build_command("-is gaMesS", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_pass_default_job_and_script) {
	build_command("-i gaMesS", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_pass_output) {
	build_command("-o fhiaims", VALID_OUTPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code == 0, "Expected success, got exit code %d",
				  exit_code);
}
END_TEST

START_TEST(test_fail_invalid_output_file) {
	build_command("-o fhiaims", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for invalid file extension for outputs");
}
END_TEST

START_TEST(test_fail_invalid_input_file) {
	build_command("-i qe", VALID_OUTPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for invalid file extension for outputs");
}
END_TEST

START_TEST(test_fail_no_arguments) {
	build_command("", "");
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for too few arguments");
}
END_TEST

START_TEST(test_fail_many_arguments) {
	build_command("-i castep opt vasp", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for too many arguments");
}
END_TEST

START_TEST(test_fail_few_arguments) {
	build_command("-", "");
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for too few arguments");
}
END_TEST

START_TEST(test_fail_no_hyphen) {
	build_command("i fhiaims freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for no hyphen");
}
END_TEST

START_TEST(test_fail_no_hyphen_duplicate) {
	build_command("ii fhiaims freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for no hyphen and duplicate options");
}
END_TEST

START_TEST(test_fail_no_hyphen_simultaneous) {
	build_command("io fhiaims freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(
		exit_code != 0,
		"Expected failure for no hyphen and simultaneous input/output");
}
END_TEST

START_TEST(test_fail_no_options_otherwise_valid) {
	build_command("- fhiaims freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for having no options but otherwise valid");
}
END_TEST

START_TEST(test_fail_simultaneous_in_out) {
	build_command("-io orca", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for simultaneous input/output call");
}
END_TEST

START_TEST(test_fail_output_with_job) {
	build_command("-o orca freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for using job with output flag");
}
END_TEST

START_TEST(test_fail_invalid_options) {
	build_command("-sj orca freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for invalid options");
}
END_TEST

START_TEST(test_fail_invalid_program) {
	build_command("-i program freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for invalid program");
}
END_TEST

START_TEST(test_fail_invalid_job) {
	build_command("-i orca job", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for invalid job type");
}
END_TEST

START_TEST(test_fail_missing_file) {
	build_command("-o orca", "missing.xyz");
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure due to missing file");
}
END_TEST

START_TEST(test_fail_output_script) {
	build_command("-os orca sp", VALID_OUTPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0,
				  "Expected failure for using option 'o' with 's'");
}
END_TEST

START_TEST(test_fail_s_input) {
	build_command("-s orca freq", VALID_INPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for using 's' without 'i'");
}
END_TEST

START_TEST(test_fail_s_output) {
	build_command("-s orca", VALID_OUTPUT_FILE);
	int exit_code = run_command(commandLine);
	ck_assert_msg(exit_code != 0, "Expected failure for using 's' without 'i'");
}
END_TEST

Suite *cli_suite(void) {
	Suite *s = suite_create("cli");

	TCase *tc_pass = tcase_create("Pass");
	TCase *tc_fail = tcase_create("Fail");

	tcase_add_test(tc_pass, test_pass_help);
	tcase_add_test(tc_pass, test_pass_upper_help);
	tcase_add_test(tc_pass, test_pass_input);
	tcase_add_test(tc_pass, test_pass_input_script);
	tcase_add_test(tc_pass, test_pass_default_job);
	tcase_add_test(tc_pass, test_pass_default_job_and_script);
	tcase_add_test(tc_pass, test_pass_output);

	tcase_add_test(tc_fail, test_fail_invalid_output_file);
	tcase_add_test(tc_fail, test_fail_invalid_input_file);
	tcase_add_test(tc_fail, test_fail_no_arguments);
	tcase_add_test(tc_fail, test_fail_many_arguments);
	tcase_add_test(tc_fail, test_fail_few_arguments);
	tcase_add_test(tc_fail, test_fail_no_hyphen);
	tcase_add_test(tc_fail, test_fail_no_hyphen_duplicate);
	tcase_add_test(tc_fail, test_fail_no_hyphen_simultaneous);
	tcase_add_test(tc_fail, test_fail_no_options_otherwise_valid);
	tcase_add_test(tc_fail, test_fail_simultaneous_in_out);
	tcase_add_test(tc_fail, test_fail_output_with_job);
	tcase_add_test(tc_fail, test_fail_invalid_options);
	tcase_add_test(tc_fail, test_fail_invalid_program);
	tcase_add_test(tc_fail, test_fail_invalid_job);
	tcase_add_test(tc_fail, test_fail_missing_file);
	tcase_add_test(tc_fail, test_fail_output_script);
	tcase_add_test(tc_fail, test_fail_s_input);
	tcase_add_test(tc_fail, test_fail_s_output);

	suite_add_tcase(s, tc_pass);
	suite_add_tcase(s, tc_fail);

	return s;
}

int main(void) {
	int failed;
	Suite *s = cli_suite();
	SRunner *sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (failed == 0) ? 0 : 1;
}
