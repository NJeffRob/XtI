#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
// #include <regex.h>
#include <stdbool.h>
#include <stdio.h>
// #include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1

// the following structs are for program and job options
struct Programs {
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
};

struct Job_types {
  char opt[30];
  char freq[30];
  char sp[30];
};

// int input_option_checking(const char *user_input);
// int program_option_checking(const char *user_input);
void help_message(void);
bool is_valid_option(char c);
bool is_duplicate_option(char *str);
void store_option(char option);
void execute_lua(const char *script);

int main(int argc, char *argv[]) {

  char help[2] = "-h";

  // if the user doesn't input anything
  // Add later: limit upper bound of argc as well
  if (argc < 2) {
    help_message();
    return FAILURE;
  }
  // prints the help message from the lua file
  // NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
  if (strcmp(argv[1], help) == SUCCESS) {
    execute_lua("help.lua");
    return SUCCESS;
  }
  // First character of the argument is '-'
  else if (argv[1][0] != '-') {
    printf("No options specified\n"); // throw help message at the end of each
    return FAILURE;
  }
  // Argument is between 2 to 5 characters
  else if (strlen(argv[1]) < 2 || strlen(argv[1]) > 5) {
    printf("Invalid number of arguments provided: %lu\n", strlen(argv[1]));
    return FAILURE;
  }
  // Argument contains only the characters 's', 'i', 'o', or 'j'
  for (int i = 1; argv[1][i] != '\0'; i++) {
    if (!is_valid_option(argv[1][i])) {
      printf("Invalid options '%c'\n", argv[1][i]);
      return FAILURE;
    }
  }
  // No duplicate options
  if (is_duplicate_option(argv[1])) {
    printf("Duplicate options are not allowed\n");
    return FAILURE;
  }

  // Store valid options
  for (int i = 1; argv[1][i] != '\0'; i++) {
    store_option(argv[1][i]);
  }

  //   char *inputOptions = argv[1];
  //    char *chemistryProgram = argv[2];
  //    char *jobOption = argv[3];
  //    char *file = argv[4];
  // Check that chemistry_program matches a value in the struct, send to lua
  // Check that job_option matches a value in the struct, send to lua
  // Check that file is valid

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

  /* this else section has to initialize a Lua API call, i.e. all the
     following code has to be able to run lua if the user returns an
     appropriate input option, check the next input if the next input is
     appropriate for the input options listed, check the next input if the
     next input is a name of a file in the directory, do stuff */
  //  lua_State *L = luaL_newstate();
  //  luaL_openlibs(L);

  // prints the help message from the lua file
  // NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
  /*  if (strcmp(argv[1], help) == SUCCESS) {
      lua_State *L = luaL_newstate();
      luaL_openlibs(L);
      if (luaL_dofile(L, "src/help.lua") == LUA_OK) {
        lua_pop(L, lua_gettop(L));
      }
      lua_close(L);
      return SUCCESS;
    }
    else {
      help_message();
    } */
  /*
  int i;
  for (i = 1; i < argc; i++) {
      input_option_checking(argv[i]);
  }
  */
  //  lua_close(L);

  return SUCCESS;
}

void help_message(void) {
  printf("Usage: xti [options] [file ...] \n"
         "Try 'xti -h' for help on getting started. \n");
}

bool is_valid_option(char c) {
  // Check if the character is 's', 'i', 'o', or 'j'
  return (c == 's' || c == 'i' || c == 'o' || c == 'j');
}

bool is_duplicate_option(char *str) {
  // Check for duplicate characters
  for (int i = 1; str[i] != '\0'; i++) {
    for (int j = i + 1; str[j] != '\0'; j++) {
      if (str[i] == str[j]) {
        return true;
      }
    }
  }
  return false;
}

// Temporarily just prints out whichever options are valid
void store_option(char option) {
  // Call Lua based on option
  switch (option) {
  case 's':
    printf("Processing option 's'\n");
    break;
  case 'i':
    printf("Processing option 'i'\n");
    break;
  case 'o':
    printf("Processing option 'o'\n");
    break;
  case 'j':
    printf("Processing option 'j'\n");
    break;
  default:
    break;
  }
}

void execute_lua(const char *script) {
  lua_State *L = luaL_newstate(); // Create new lua state
  luaL_openlibs(L);               // Open lua libraries

  if (luaL_dofile(L, script) != LUA_OK) {
    fprintf(stderr, "Error running Lua script: %s\n", lua_tostring(L, -1));
  }

  // Do we need to clear the lua stack after all successful operations?
  // if (luaL_dofile(L, "src/help.lua") == LUA_OK) {
  //  lua_pop(L, lua_gettop(L));
  //}

  lua_close(L);
}

/*
// Regex; need to make sure that the first argument is along the lines of
// -p/s/i/o/j
int input_option_checking(const char *user_input) {
  regex_t regex;

  // regex that Lawrence came up with that works <3
  const char *input_options =
      "^-(?!.*(.).*\1)[sioj]+(?: (?!.*(.).*\1)[sioj]+)*$";

  // Check if input option compiles regex
  int regexCompile = regcomp(&regex, input_options, REG_EXTENDED | REG_NOSUB);

  if (regexCompile != SUCCESS) {
    char errorBuffer[100];
    regerror(regexCompile, &regex, errorBuffer, sizeof(errorBuffer));
    fprintf(stderr, "Regex compile failed: %s\n", errorBuffer);
    return FAILURE; // return FAILURE if regex compilation fails
  }

  int xti_input_options = regexec(&regex, user_input, 0, NULL, 0);
  regfree(&regex);

  if (xti_input_options == SUCCESS) {
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

int program_option_checking(const char *user_input) {}
*/
