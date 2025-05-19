#include "../include/file_handler.h"
#include "../include/util.h"
#include <check.h>

START_TEST(test_pass_xyz) {
	ck_assert(check_file_extension("pass.xyz", ".xyz"));
	ck_assert(check_file_extension("pass.sh", ".sh"));
	ck_assert(check_file_extension("pass.log", ".log"));
}
END_TEST

START_TEST(test_fail_xyz) {
	ck_assert(!check_file_extension("fail.xy", ".xyz"));
	ck_assert(!check_file_extension("fail.xyz", ".sh"));
	ck_assert(!check_file_extension("fail.lua", "c"));
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
