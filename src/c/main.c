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

#define PATH_TO_HELP  "src/lua/help.lua"
#define PATH_TO_INPUT "src/lua/xyz_to_input_converter.lua"
// #define PATH_TO_OUTPUT "src/lua/output_to_xyz_converter.lua"
#define PATH_TO_SH "src/lua/sh_generator.lua"

#define DEFAULT_JOB_TYPE "sp"

const char *valid_program[] = {"abinit",   "gamess",  "qe",	   "orca",
							   "gaussian", "fhiaims", "qchem", "siesta",
							   "vasp",	   "castep"};

const char *valid_job[] = {"opt", "freq", "sp"};

int error_fail_message(const char *message, ...);

int main(int argc, char *argv[]) {
	// Set argv constants
	char *option = convert_to_lower(argv[1]);
	char *chemistry_program = convert_to_lower(argv[2]);
	char *job_type;
	const char *file_path = argv[argc - 1];

	// Create new lua state and open lua libraries
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Go through various checks to see if command line options are valid
	char help[3] = "-h";
	if (argc < 2 || argc > 5) {
		return error_fail_message("Wrong number of arguments\n");
	}
	if (strcmp(option, help) == SUCCESS) {
		// Print the help message
		exec_lua_script(L, PATH_TO_HELP);
		return SUCCESS;
	}
	// First character of the argument is '-'
	else if (option[0] != '-') {
		return error_fail_message("No options specified\n");
	}
	// Options argument has length of 2 || 3
	else if (!is_valid_length(option, 2, 3)) {
		return error_fail_message("Invalid number of options\n");
	}
	// Argument contains only valid options ('i', 'o', 's')
	for (int i = 1; option[i] != '\0'; i++) {
		if (!is_valid_option(option[i])) {
			return error_fail_message("Invalid options '%c'\n", option[i]);
		}
	}
	// Option 's' can only be used if 'i' was used
	if (strchr(option, 's') && !strchr(option, 'i')) {
		printf("Error: Option 's' requires the option 'i'\n");
		return FAILURE;
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
		return error_fail_message("Invalid program\n");
	}
	// Check if string is in the static array
	else if (!match_str(chemistry_program, valid_program,
						sizeof(valid_program))) {
		return error_fail_message("Invalid program\n");
	}

	// Check if the file is valid by accessing
	FILE *file = fopen(file_path, "r");
	if (file == NULL) {
		if (errno == ENOENT) {
			printf("Error: File \"%s\" does not exist.\n", file_path);
		} else if (errno == EACCES) {
			printf("Error: Permission denied for file \"%s\".\n", file_path);
		} else {
			printf("Error: cannot open file");
		}
		return FAILURE;
	}
	fclose(file);

	// option_output = true, then move to file handling
	if (option_output) {
		// Exit if incorrect number of arguments
		if (argc != 4) {
			return error_fail_message("Wrong number of arguments for output\n");
		}

		// Valid file extensions for -o
		if (!check_file_extension(file_path, ".log")) {
			return error_fail_message(
				"The file \"%s\" does not have a valid extension for outputs\n",
				file_path);
		}

		printf("Generate output file\n");
		printf("Input success: \"%s\"\n", file_path);
	}
	// option_input = true, set job_type if specified or default to "sp"
	if (option_input) {
		bool job_memory = false;

		if (argc < 5) {
			// Use default job_type
			job_type = DEFAULT_JOB_TYPE;
		} else {
			job_type = convert_to_lower(argv[3]);
			job_memory = true;
			if (!is_valid_length(job_type, 2, 4)) {
				return error_fail_message("Invalid job type\n");
			}
			// Check if string is in the static array
			else if (!match_str(job_type, valid_job, sizeof(valid_job))) {
				return error_fail_message("Invalid job type\n");
			}
		}

		// Check if .xyz file, (temporary). Should be called if option_i = true
		if (!check_file_extension(file_path, ".xyz")) {
			return error_fail_message(
				"The file \"%s\" does not have a .xyz extension\n", file_path);
		}

		// Check if option s specified
		if (option_script) {
			// Determine and call the appropriate lua function
			snprintf(lua_func, sizeof(lua_func), "%s_sh", chemistry_program);
			exec_lua_function(L, PATH_TO_SH, lua_func, file_path, job_type);
			// Add in error handling
		}

		// Input success
		snprintf(lua_func, sizeof(lua_func), "xyz_to_%s", chemistry_program);
		exec_lua_function(L, PATH_TO_INPUT, lua_func, file_path, job_type);
		// Add in error handling

		// printf("Generate input file\n");
		// printf("Input success: \"%s\"\n", file_path);

		// Pass job_type to Lua OR can execute_lua

		// Free memory from job_type if allocate
		if (job_memory) {
			free(job_type);
		}
	}

	// Pass chemistry_program to Lua OR can execute_lua

	// Free memory from chemistry_program
	free(chemistry_program);

	// Pass file_path

	// Free memory from option
	free(option);

	// Free memory from Lua
	lua_close(L);

	printf("Done!\n");
	return SUCCESS;
}

// Fail out gracefully and give help message
int error_fail_message(const char *message, ...) {
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	help_prompt();
	return FAILURE;
}
