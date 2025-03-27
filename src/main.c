#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
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

int input_option_checking(const char *user_input);
int program_option_checking(const char *user_input);

int main(int argc, char *argv[]) {

  // test
  printf("%s\n", argv[1]);
  // printf("%d", input_option_checking(argv[1]));

  char help[5] = "-h";

  // if the user doesn't input anything
  if (argc < 2) {
    printf("Usage: xti [options] [file ...] \n"
           "Try 'xti -h' for help on getting started. \n");

    return FAILURE;
  } else {
    //   char *inputOptions = argv[1];
    //    char *chemistryProgram = argv[2];
    //    char *jobOption = argv[3];
    //    char *file = argv[4];

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
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // prints the help message from the lua file
    // NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
    if (strcmp(argv[1], help) == SUCCESS) {
      if (luaL_dofile(L, "src/help.lua") == LUA_OK) {
        lua_pop(L, lua_gettop(L));
      }
    } else if (input_option_checking(argv[1]) == SUCCESS) {
      printf("%s\n", argv[1]);
    } else {
      printf("Usage: xti [options] [file ...] \n"
             "Try 'xti -h' for help on getting started. \n");
    }
    /*
    int i;
    for (i = 1; i < argc; i++) {
        input_option_checking(argv[i]);
    }
    */
    lua_close(L);

    return SUCCESS;
  }
}

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
