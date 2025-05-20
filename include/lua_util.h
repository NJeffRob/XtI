#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include <lauxlib.h>
#include <limits.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Determine the path to the lua files
void build_lua_path(char *dest, const char *script);

void exec_lua_script(lua_State *L, const char *script);
void exec_lua_function(lua_State *L, const char *script, const char *func_name,
					   const char *arg1, const char *arg2);

#endif
