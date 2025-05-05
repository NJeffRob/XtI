import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))
TEST_DIR = os.path.join(ROOT_DIR, "tests")
MAKEFILE_PATH = os.path.join(ROOT_DIR, "Makefile")


def create_test_file(filename: str):
    path = os.path.join(TEST_DIR, filename)
    if os.path.exists(path):
        print(f"[!] Test file already exists: {filename}")
        return False
    base = filename.split(".")[0]
    with open(path, "w") as f:
        _ = f.write(f"""#include "../include/{base}.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

START_TEST(sample_test) {{
    ck_assert_int_eq(1, 1);
}} END_TEST

Suite *{base}_suite(void) {{
  Suite *s = suite_create("{base}");

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
    Suite *s = {base}_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? 0 : 1;
}}
""")
    print(f"[+] Created test file: {filename}")
    return True


def update_makefile(filename: str):
    base = filename.split(".")[0]
    test_target = f"test_{base}"
    test_exec = f"$(BIN_DIR)/{test_target}"
    test_path = f"$(TEST_DIR)/{filename}"

    with open(MAKEFILE_PATH, "r") as f:
        lines = f.readlines()

    if any(line.startswith(f"{test_target}:") for line in lines):
        print(f"[!] Makefile already contains rule for: {test_target}")
        return

    # Find where to insert new test rule (after existing test_ targets)
    # Find the index of the composite "test:" target
    composite_test_idx = next(
        (i for i, line in enumerate(lines) if line.strip().startswith("test:")), None
    )

    if composite_test_idx is None or composite_test_idx < 2:
        print("[!] Could not find valid 'test:' target to insert before")
        return

    # Insert two lines above 'test:' to maintain spacing and comments
    insert_index = composite_test_idx - 2

    rule_block = f"""\n{test_target}: {test_path} src/c/{base}.c
\t@mkdir -p $(BIN_DIR)
\t$(CC) {test_path} src/c/{base}.c -o {test_exec} $(shell pkg-config --cflags --libs check)
\t{test_exec}\n"""

    # Insert test rule
    lines.insert(insert_index, rule_block)

    # Update test: line
    for i, line in enumerate(lines):
        if line.startswith("test:"):
            if test_target not in line:
                lines[i] = line.strip() + f" {test_target}\n"
                print(f"[+] Added '{test_target}' to 'test:' target")
            break

    # Update .PHONY
    for i, line in enumerate(lines):
        if line.startswith(".PHONY:"):
            if test_target not in line:
                lines[i] = line.strip() + f" {test_target}\n"
                print(f"[+] Added '{test_target}' to '.PHONY'")
            break

    with open(MAKEFILE_PATH, "w") as f:
        f.writelines(lines)

    print(f"[+] Updated Makefile with rule for: {test_target}")


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
