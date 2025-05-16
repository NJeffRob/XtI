#include "../../include/lua_util.h"

// Function to execute lua script
void exec_lua_script(lua_State *L, const char *script) {
	if (luaL_dofile(L, script) != LUA_OK) {
		fprintf(stderr, "Error running Lua script: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

// Function to execute lua function within a .lua file
void exec_lua_function(lua_State *L, const char *script, const char *func_name,
					   const char *arg1, const char *arg2) {
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
	// Push function arguments from C to Lua
	lua_pushstring(L, arg1); // io_name
	lua_pushstring(L, arg2); // calc_type
	// Call lua function with arguments
	if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
		fprintf(stderr, "Error running Lua function '%s': %s\n", func_name,
				lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

