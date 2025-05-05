#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <string.h>

// store and send to lua
int main() {
  // xti -psij chemistry_program job_option
  char input[100] = "xti -psij some_program some_job";

  // chemistry_program and job_option param
  char chemistryProgram[50], jobOption[50];

  // Can I ignore the other stuff and pick these out?
  // Split the string by spaces
  char *splitInput = strtok(input, " ");

  // Remove the "xti -psij"
  splitInput = strtok(NULL, " ");
  splitInput = strtok(NULL, " ");

  // Point splitInput to chemistryProgram
  strcpy(chemistryProgram, splitInput);

  // Point to jobOption
  splitInput = strtok(NULL, " ");
  strcpy(jobOption, splitInput);

  // Print the parameters in C
  // printf("Chemistry program: %s\n", chemistryProgram);
  // printf("Job option: %s\n", jobOption);

  // Initialize lua
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  // Push variables to lua stack
  lua_pushstring(L, chemistryProgram);
  lua_setglobal(L, "chemistryProgram");

  lua_pushstring(L, jobOption);
  lua_setglobal(L, "jobOption");

  // Execute the .lua file
  const char *lua_script_path = "pass_argument.lua";

  if (luaL_dofile(L, lua_script_path) != LUA_OK) {
    fprintf(stderr, "Error running Lua script: %s\n", lua_tostring(L, -1));
  }

  // Close the lua state
  lua_close(L);
}
