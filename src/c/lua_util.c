#include "../../include/lua_util.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// Function to execute lua script
void exec_lua_script(lua_State *L, const char *script) {
	if (luaL_dofile(L, script) != LUA_OK) {
		fprintf(stderr, "Error running Lua script: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

// Function to execute lua function within a .lua file
void exec_lua_function(lua_State *L, const char *script, const char *func_name,
					   int argc, const char *argv[]) {
	if (luaL_dofile(L, script) != LUA_OK) {
		fprintf(stderr, "Error loading Lua script: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return;
	}
	// Push the function onto lua stack
	lua_getglobal(L, func_name);
	if (!lua_isfunction(L, -1)) {
		fprintf(stderr, "Lua error: '%s' is not a function\n", func_name);
		lua_pop(L, 1);
		return;
	}
	// Push arguments onto lua stack
	for (int i = 0; i < argc; i++) {
		lua_pushstring(L, argv[i]);
	}
	// Call lua function with arguments
	if (lua_pcall(L, argc, 0, 0) != LUA_OK) {
		fprintf(stderr, "Error running Lua function '%s': %s\n", func_name,
				lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

// Deprecated
// File to pass a variable into a .lua file
void pass_argument_lua(lua_State *L, const char *str, const char *global_var,
					   const char *lua_path) {
	lua_pushstring(L, str);
	lua_setglobal(L, global_var);
	// Execute the Lua script
	const char *lua_script = lua_path;

	if (luaL_dofile(L, lua_script) != LUA_OK) {
		fprintf(stderr, "Error passing argument to Lua script: %s\n",
				lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}
