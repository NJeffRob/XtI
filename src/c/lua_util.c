#include "../../include/lua_util.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// Temporarily just prints out whichever options are valid
// Add another param, char *script, then execute_lua(script) instead of
// printing.
void option_to_lua(char option) {
  // Call Lua based on option
  switch (option) {
  case 's':
    printf("Lua function 's'\n");
    break;
  case 'i':
    printf("Lua function 'i'\n");
    break;
  case 'o':
    printf("Lua function 'o'\n");
    break;
  case 'j':
    printf("Lua function 'j'\n");
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

  // Only need to clear lua stack if leaving lua state open
  // if (luaL_dofile(L, "src/help.lua") == LUA_OK) {
  //  lua_pop(L, lua_gettop(L));
  //}

  lua_close(L);
}

void pass_argument_lua(const char *str, const char *global_var,
                       const char *lua_path) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushstring(L, str);
  lua_setglobal(L, global_var);

  // Execute the Lua script
  const char *lua_script = lua_path;

  if (luaL_dofile(L, lua_script) != LUA_OK) {
    fprintf(stderr, "Error passing argument to Lua script: %s\n",
            lua_tostring(L, -1));
  }

  lua_close(L);
}
