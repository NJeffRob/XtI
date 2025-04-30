#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../../include/file_handler.h"
#include "../../include/lua_util.h"
#include "../../include/option_handler.h"
#include "../../include/util.h"

#define SUCCESS 0
#define FAILURE 1

const char *valid_program[] = {"abinit",   "gamess",  "qe",    "orca",
                               "gaussian", "fhiaims", "qchem", "siesta",
                               "vasp",     "castep"};

const char *valid_job[] = {"opt", "freq", "sp"};

int main(int argc, char *argv[]) {
  // Call option handling
  const char *option = argv[1];
  char help[3] = "-h";
  if (argc < 2 || argc > 5) {
    help_prompt();
    return FAILURE;
  }
  if (strcmp(option, help) == SUCCESS) {
    // Print the help message from the Lua file
    // NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
    execute_lua("src/lua/help.lua");
    return SUCCESS;
  }
  // First character of the argument is '-'
  else if (option[0] != '-') {
    printf("No options specified\n"); // throw help message at the end of each
    help_prompt();
    return FAILURE;
  }
  // Argument is between 2 to 5 characters
  else if (!is_valid_length(option, 2, 5)) {
    printf("Invalid number of options\n");
    help_prompt();
    return FAILURE;
  }
  // Argument contains only the characters 's', 'i', 'o', or 'j'
  for (int i = 1; option[i] != '\0'; i++) {
    if (!is_valid_option(option[i])) {
      printf("Invalid options '%c'\n", option[i]);
      help_prompt();
      return FAILURE;
    }
  }
  // No duplicate options
  if (is_duplicate_option(option)) {
    printf("Duplicate options are not allowed\n");
    help_prompt();
    return FAILURE;
  }
  // Options 'i' and 'o' are mutually exclusive
  if (strchr(option, 'i') && strchr(option, 'o')) {
    printf("Error: Options 'i' and 'o' cannot be called simultaneously\n");
    return FAILURE;
  }
  // Option 'j' can only be used if 'i' was used
  if (strchr(option, 'j') && !strchr(option, 'i')) {
    printf("Error: Options 'j' requires the option 'i'\n");
    return FAILURE;
  }

  // Call lua API for valid options
  // Temporarily just prints out whichever options are valid
  // Add another param, char *script, then execute_lua(script) instead of
  // printing.
  for (int i = 1; option[i] != '\0'; i++) {
    switch (option[i]) {
    case 's':
      printf("s: Generate .sh file\n");
      break;
    case 'i':
      printf("i: Input .xyz\n");
      break;
    case 'o':
      printf("o: Output .xyz\n");
      break;
    case 'j':
      printf("j: Specify job\n");
      break;
    default:
      break;
    }
  }

  // Call program handling
  const char *chemistry_program = argv[2];
  if (!is_valid_length(chemistry_program, 2, 9)) {
    printf("Invalid program\n");
    help_prompt();
    return FAILURE;
  }
  // Check if string is in the static array
  else if (!match_str(chemistry_program, valid_program,
                      sizeof(valid_program))) {
    printf("Invalid program\n");
    help_prompt();
    return FAILURE;
  }
  // Pass chemistry_program to Lua
  else {
    pass_argument_lua(chemistry_program, "chemistry_program",
                      "tests/pass_argument.lua");
  }

  // Call job handling
  const char *job_type = argv[3];
  if (!is_valid_length(job_type, 2, 4)) {
    printf("Invalid job type\n");
    help_prompt();
    return FAILURE;
  }
  // Check if string is in the static array
  else if (!match_str(job_type, valid_job, sizeof(valid_job))) {
    printf("Invalid job type\n");
    help_prompt();
    return FAILURE;
  }
  // Pass job_type to Lua
  else {
    pass_argument_lua(job_type, "job_type", "tests/pass_argument.lua");
  }

  // Call file handling
  const char *file_name = argv[4];
  FILE *file = fopen(file_name, "r");
  // Check if file is valid and accessible
  if (file == NULL) {
    if (errno == ENOENT) {
      printf("Error: File \"%s\" does not exist.\n", file_name);
    } else if (errno == EACCES) {
      printf("Error: Permission denied for file \"%s\".\n", file_name);
    } else {
      perror("Error: cannot open file");
      printf("Error: cannot open file");
    }
    return FAILURE;
  }
  fclose(file);
  // Check if .xyz file, then send to lua
  if (!file_xyz_extension(file_name)) {
    printf("Error: The file \"%s\" does not have a .xyz extension.\n",
           file_name);
    return 1;
  }
  // If all checks pass
  printf("The file \"%s\" is valid and has a .xyz extension.\n", file_name);
  return SUCCESS;
}
