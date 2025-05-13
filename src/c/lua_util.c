#include "../../include/lua_util.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// Function to execute lua script
void exec_lua_script(lua_State *L, const char *script) {
	if (luaL_dofile(L, script) != LUA_OK) {
		fprintf(stderr, "Error running Lua script: %s\n", lua_tostring(L, -1));
		lua_close(L);
	}
}

// Function to execute lua function within a .lua file
void exec_lua_function(lua_State *L, const char *script,
					   const char *function_name) {
	if (luaL_dofile(L, script) != LUA_OK) {
		fprintf(stderr, "Error loading Lua script: %s\n", lua_tostring(L, -1));
		lua_close(L);
		return;
	}
	// Push the function onto the stac
	lua_getglobal(L, function_name);
	if (!lua_isfunction(L, -1)) {
		fprintf(stderr, "Lua error: '%s' is not a function\n", function_name);
		lua_close(L);
		return;
	}
	// Call the function with 0 arguments, expecting 0 results
	if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
		fprintf(stderr, "Error running Lua function '%s': %s\n", function_name,
				lua_tostring(L, -1));
	}
}

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
	}
}
