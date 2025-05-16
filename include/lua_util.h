#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

void exec_lua_script(lua_State *L, const char *script);
void exec_lua_function(lua_State *L, const char *script, const char *func_name,
					   const char *arg1, const char *arg2);
void pass_argument_lua(lua_State *L, const char *str, const char *global_var);

#endif
