import os
import sys
import re

# Paths relative to script location
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))
TEST_DIR = os.path.join(SCRIPT_DIR, '..', 'tests')
MAKEFILE_PATH = os.path.join(SCRIPT_DIR, '..', 'Makefile')

def create_test_file(filename):
    test_path = os.path.join(TEST_DIR, filename)
    if os.path.exists(test_path):
        print(f"Test file '{filename}' already exists.")
        return False
    with open(test_path, 'w') as f:
        f.write(f"""#include "../include/.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

START_TEST(sample_test) {{
  ck_assert_int_eq(1, 1);
}} END_TEST

Suite *{filename.split('.')[0]}_suite(void) {{
  Suite *s = suite_create("{filename.split('.')[0]}");

  TCase *tc_pass = tcase_create("Pass");
  TCase *tc_fail = tcase_create("Fail");

  tcase_add_test(tc_pass, sample_test);

  tcase_add_test(tc_fail, sample_test);

  suite_add_tcase(s, tc_pass);
  suite_add_tcase(s, tc_fail);

  return s;
}}

int main(void) {{
    int failed = 0;
    Suite *s = {filename.split('.')[0]}_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? 0 : 1;
}}
""")
    print(f"Created test file: {test_path}")
    return True

def update_makefile(filename):
    base = filename.split(".")[0]
    binary_path = f"$(BIN_DIR)/{base}"
    test_path = f"$(TEST_DIR)/{filename}"
    rule = f"""
{base}: {test_path}
\t@mkdir -p $(BIN_DIR)
\t$(CC) {test_path} -o {binary_path} $(shell pkg-config --cflags --libs check)
\t{binary_path}
"""

    with open(MAKEFILE_PATH, "r") as f:
        makefile = f.read()

    if re.search(rf"^{base}:", makefile, re.MULTILINE):
        print(f"[!] Makefile already has a rule for: {base}")
        return

    # Append rule to end
    with open(MAKEFILE_PATH, "a") as f:
        f.write(rule)

    print(f"[+] Appended rule for: {base}")

    # Update the 'test:' target
    if "test:" in makefile:
        new_makefile_lines = []
        for line in makefile.splitlines():
            if line.startswith("test:"):
                if base not in line:
                    line = line.strip() + f" {base}"
                    print(f"[+] Added '{base}' to 'test:' target")
            new_makefile_lines.append(line)
        with open(MAKEFILE_PATH, "w") as f:
            f.write("\n".join(new_makefile_lines) + "\n")

def main():
    if len(sys.argv) != 2:
        print("Usage: generate_test.py <filename.c>")
        return

    filename = sys.argv[1]
    if not filename.endswith(".c"):
        print("Error: Test filename must end in .c")
        return

    os.makedirs(TEST_DIR, exist_ok=True)

    if create_test_file(filename):
        update_makefile(filename)

if __name__ == "__main__":
    main()
