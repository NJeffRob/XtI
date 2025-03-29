#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include <stdbool.h>
#include <stdio.h>
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

const char *valid_program[] = {"abinit",   "gamess",  "qe",    "orca",
                               "gaussian", "fhiaims", "qchem", "siesta",
                               "vasp",     "castep"};

struct Job_types {
  char opt[30];
  char freq[30];
  char sp[30];
};

void help_prompt(void);
bool is_valid_option(char c);
bool is_duplicate_option(const char *str);
void option_to_lua(char option);
void execute_lua(const char *script);

int main(int argc, char *argv[]) {
  // option
  char help[3] = "-h";
  const char *option = argv[1]; // Store argv[1] in a variable
  // Add later: limit upper bound of argc as well
  if (argc < 2) {
    help_prompt();
    return FAILURE;
  }
  // prints the help message from the lua file
  // NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
  if (strcmp(option, help) == SUCCESS) {
    execute_lua("lua/help.lua");
    return SUCCESS;
  }
  // First character of the argument is '-'
  else if (option[0] != '-') {
    printf("No options specified\n"); // throw help message at the end of each
    help_prompt();
    return FAILURE;
  }
  // Argument is between 2 to 5 characters
  else if (strlen(option) < 2 || strlen(option) > 5) {
    printf("Invalid number of arguments provided: %lu\n", strlen(option));
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

  // chemistry_program = argv[2]
  // If valid, store and send chemistry_program to Lua

  // >2, <10

  // check if in the struct or whatever

  // pass_argument to lua


  // job_option = argv[3]
  // if valid, pass argumetn to lua


  // file = argv[4]


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

 
  return SUCCESS;
}

void help_prompt(void) {
  printf("Usage: xti [options] [file ...] \n"
         "Try 'xti -h' for help on getting started. \n");
}

bool is_valid_option(char c) {
  // Check if the character is 's', 'i', 'o', or 'j'
  return (c == 's' || c == 'i' || c == 'o' || c == 'j');
}

bool is_duplicate_option(const char *str) {
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
// Add another param, char *script, then execute_lua(script) instead of
// printing.
void option_to_lua(char option) {
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
