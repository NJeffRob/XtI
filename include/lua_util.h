#ifndef LUA_UTIL_H
#define LUA_UTIL_H

void execute_lua(const char *script);
void pass_argument_lua(const char *str, const char *global_var,
					   const char *lua_path);

#endif
