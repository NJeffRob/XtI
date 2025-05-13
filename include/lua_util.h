#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include <lua.h>
void exec_lua_script(lua_State *L, const char *script);
void exec_lua_function(lua_State *L, const char *script, const char *func_name,
					   int argc, const char *argv[]);
void pass_argument_lua(lua_State *L, const char *str, const char *global_var,
					   const char *lua_path);

#endif
