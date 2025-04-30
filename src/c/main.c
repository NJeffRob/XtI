#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../../include/file_handler.h"
#include "../../include/lua_util.h"
#include "../../include/option_handler.h"
#include "../../include/util.h"

#define SUCCESS 0
#define FAILURE 1

// the following structs are for program and job options
/*struct Programs {
  char abinit[7];
  char gamess[7];
  char qe[3]; // quantum espresso
  char orca[5];
  char gaussian[9];
  char fhiaims[8];
  char qchem[6];
  char siesta[7];
  char vasp[5];
  char castep[7];
};*/

const char *valid_program[] = {"abinit",   "gamess",  "qe",    "orca",
                               "gaussian", "fhiaims", "qchem", "siesta",
                               "vasp",     "castep"};

/*struct Job_types {
  char opt[30];
  char freq[30];
  char sp[30];
};*/

const char *valid_job[] = {"opt", "freq", "sp"};
void help_prompt(void);


// Rearrange all case handling to check for any failures with all of
// option/program/job first, then perform the expected operations

int main(int argc, char *argv[]) {

  // option_handling
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

  // Call lua API for valid options
  for (int i = 1; option[i] != '\0'; i++) {
    option_to_lua(option[i]);
  }

  // program_handling
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

  // job_handling
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

  // file_handling
  const char *file_name = argv[4];
  FILE *file = fopen(file_name, "r");

  // Check if .xyz file, then send to lua
  if (!file_xyz_extension(file_name)) {
    fprintf(stderr, "Error: The file \"%s\" does not have a .xyz extension.\n",
            file_name);
    return 1;
  }

  // Check if file is valid and accessible
  if (file == NULL) {
    if (errno == ENOENT) {
      fprintf(stderr, "Error: File \"%s\" does not exist.\n", file_name);
    } else if (errno == EACCES) {
      fprintf(stderr, "Error: Permission denied for file \"%s\".\n", file_name);
    } else {
      perror("Error opening file");
    }
    return FAILURE;
  }
  fclose(file);

  // If all checks pass
  printf("The file \"%s\" is valid and has a .xyz extension.\n", file_name);

  /*LAWRENCE: here is the most complicated example input I can think of:

  xti -psij orca freq test.xyz

  More broadly, this can be thought of as:

  our_program -options chemistry_program job_option file.txt

  there are a few things I'd like to know:
  1) can the argument "chemistry_program" be stored and sent to Lua files?
  2) can the argument "job_option" be stored and sent to Lua files?

  If 1/2 are yes's, there might be an easier way for me to do some things in
  Lua.
  */
  return SUCCESS;
}

void help_prompt(void) {
  printf("Usage: xti -[options] [program] [job] [file...] \n"
         "Try 'xti -h' for help on getting started. \n");
}


