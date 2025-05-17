#include <errno.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/file_handler.h"
#include "../../include/lua_util.h"
#include "../../include/option_handler.h"
#include "../../include/util.h"

#define SUCCESS 0
#define FAILURE 1

#define PATH_TO_HELP   "src/lua/help.lua"
#define PATH_TO_INPUT  "src/lua/xyz_to_input_converter.lua"
#define PATH_TO_OUTPUT "src/lua/output_to_xyz_converter.lua"
#define PATH_TO_SH	   "src/lua/sh_generator.lua"

#define DEFAULT_CALC_TYPE "sp"

const char *valid_program[] = {"gamess", "orca", "gaussian", "fhiaims"};
const char *valid_calc[] = {"opt", "freq", "sp"};

int error_fail_message(const char *message, ...);

int main(int argc, char *argv[]) {
	// Set argv constants
	char *option = convert_to_lower(argv[1]);
	char *chemistry_program = convert_to_lower(argv[2]);
	char *calc_type;
	const char *file_path = argv[argc - 1];

	// Create new lua state and open lua libraries
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Go through various checks to see if command line options are valid
	char help[3] = "-h";
	if (argc < 2 || argc > 5) {
		return error_fail_message("Invalid number of arguments\n");
	}
	if (strcmp(option, help) == SUCCESS) {
		// Print the help message
		exec_lua_script(L, PATH_TO_HELP);
		return SUCCESS;
	}
	// First character of the argument is '-'
	else if (option[0] != '-') {
		return error_fail_message("No option argument specified\n");
	}
	// Options argument has length of 2 || 3
	else if (!is_valid_length(option, 2, 3)) {
		return error_fail_message("Invalid number of options\n");
	}
	// Argument contains only valid options ('i', 'o', 's')
	for (int i = 1; option[i] != '\0'; i++) {
		if (!is_valid_option(option[i])) {
			return error_fail_message("Invalid option '%c' specified\n",
									  option[i]);
		}
	}
	// Option 's' can only be used if 'i' was used
	if (strchr(option, 's') && !strchr(option, 'i')) {
		return error_fail_message("Option 's' requires the option 'i'\n");
	}

	// Booleans to store options
	bool option_input = false;
	bool option_output = false;
	bool option_script = false;

	for (int i = 1; option[i] != '\0'; i++) {
		switch (option[i]) {
		case 'i':
			option_input = true;
			break;
		case 'o':
			option_output = true;
			break;
		case 's':
			option_script = true;
		default:
			break;
		}
	}

	// Variable lua function to call
	char lua_func[64];

	// Check if program is defined in the chemistry_program array
	if (!is_valid_length(chemistry_program, 2, 9)) {
		return error_fail_message("Invalid program argument\n");
	}
	// Check if string is in the static array
	else if (!match_str(chemistry_program, valid_program,
						sizeof(valid_program))) {
		return error_fail_message("Invalid program argument\n");
	}

	// Check if the file is valid by accessing
	FILE *file = fopen(file_path, "r");
	if (file == NULL) {
		if (errno == ENOENT) {
			printf("File \"%s\" does not exist.\n", file_path);
		} else if (errno == EACCES) {
			printf("File permission denied for \"%s\".\n", file_path);
		} else {
			printf("File cannot be opened");
		}
		return FAILURE;
	}
	fclose(file);

	// Set to true if the calc_type is specified to later free memory
	bool calc_memory = false;
	// Check if calc_type is specified, and if not then set to default
	if (argc < 5) {
		// Use default calc_type
		calc_type = DEFAULT_CALC_TYPE;
	} else {
		calc_memory = true;
		calc_type = convert_to_lower(argv[3]);
		if (!is_valid_length(calc_type, 2, 4)) {
			return error_fail_message("Invalid calc type argument\n");
		}
		// Check if the argument is in the static array
		else if (!match_str(calc_type, valid_calc, sizeof(valid_calc))) {
			return error_fail_message("Invalid calc type argument\n");
		}
	}

	// -o: Check for file extension
	if (option_output) {
		// Valid file extensions for -o (.out or .log)
		if (!check_file_extension(file_path, ".log") &&
			!check_file_extension(file_path, ".out")) {
			return error_fail_message(
				"The file \"%s\" does not have a valid extension for outputs\n",
				file_path);
		}
		// Call the appropriate function based on command line arguments
		snprintf(lua_func, sizeof(lua_func), "%s_to_xyz", chemistry_program);
		exec_lua_function(L, PATH_TO_OUTPUT, lua_func, file_path, calc_type);
	}
	// -i: Check for file extension and if -s was specified
	if (option_input) {
		// Check if the file extension is .xyz
		if (!check_file_extension(file_path, ".xyz")) {
			return error_fail_message(
				"The file \"%s\" does not have a .xyz extension\n", file_path);
		}
		// Option s specified
		if (option_script) {
			// Determine and call the appropriate lua function
			snprintf(lua_func, sizeof(lua_func), "%s_sh", chemistry_program);
			exec_lua_function(L, PATH_TO_SH, lua_func, file_path, calc_type);
		}
		// Remove the file extension from file_path
		char *no_ext = remove_file_extension(file_path);
		// Call the appropriate function based on command line arguments
		snprintf(lua_func, sizeof(lua_func), "xyz_to_%s", chemistry_program);
		exec_lua_function(L, PATH_TO_INPUT, lua_func, no_ext, calc_type);
		// Free no_ext
		free(no_ext);
	}

	// Free memory of option, chemistry_program, calc_type and Lua
	free(option);
	free(chemistry_program);
	if (calc_memory) {
		free(calc_type);
	}
	lua_close(L);

	// Success messages are printed from Lua
	return SUCCESS;
}

// Fail out gracefully and give help message
int error_fail_message(const char *message, ...) {
	printf("Error: ");

	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	help_prompt();

	return FAILURE;
}
