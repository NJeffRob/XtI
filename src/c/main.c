#include <errno.h>
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

#define DEFAULT_JOB_TYPE "sp"

const char *valid_program[] = {"abinit",   "gamess",  "qe",	   "orca",
							   "gaussian", "fhiaims", "qchem", "siesta",
							   "vasp",	   "castep"};

const char *valid_job[] = {"opt", "freq", "sp"};

int error_fail_message(const char *message, ...);

int main(int argc, char *argv[]) {
	// Set argv constants
	char *option = convert_to_lower(argv[1]);
	// const char *option = argv[1];
	char *chemistry_program = convert_to_lower(argv[2]);
	char *job_type;
	const char *file_name = argv[argc - 1];

	// Call option handling
	char help[3] = "-h";
	if (argc < 2 || argc > 5) {
		return error_fail_message("Wrong number of arguments\n");
	}
	if (strcmp(option, help) == SUCCESS) {
		// Print the help message from the Lua file
		// NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
		execute_lua("src/lua/help.lua");
		return SUCCESS;
	}
	// First character of the argument is '-'
	else if (option[0] != '-') {
		return error_fail_message("No options specified\n");
	}
	// Options argument has length of 2
	else if (!is_valid_length(option, 2, 2)) {
		return error_fail_message("Invalid number of options\n");
	}
	// Argument contains only the characters 'i' or 'o'
	for (int i = 1; option[i] != '\0'; i++) {
		if (!is_valid_option(option[i])) {
			return error_fail_message("Invalid options '%c'\n", option[i]);
		}
	}

	// Call lua API for valid options
	// Temporarily just prints out whichever options are valid
	// Add another param, char *script, then execute_lua(script) instead of
	// printing.
	bool option_input = false;
	bool option_output = false;

	for (int i = 1; option[i] != '\0'; i++) {
		switch (option[i]) {
		case 'i':
			option_input = true;
			break;
		case 'o':
			option_output = true;
			break;
		// valid -s, -si/is (unique from -i). invalid: -so
		default:
			break;
		}
	}

	// Call program handling
	if (!is_valid_length(chemistry_program, 2, 9)) {
		return error_fail_message("Invalid program\n");
	}
	// Check if string is in the static array
	else if (!match_str(chemistry_program, valid_program,
						sizeof(valid_program))) {
		return error_fail_message("Invalid program\n");
	}

	// Call file handling
	FILE *file = fopen(file_name, "r");
	// Check if file is valid and accessible
	if (file == NULL) {
		if (errno == ENOENT) {
			printf("Error: File \"%s\" does not exist.\n", file_name);
		} else if (errno == EACCES) {
			printf("Error: Permission denied for file \"%s\".\n", file_name);
		} else {
			perror("Error: cannot open file");
			//      printf("Error: cannot open file");
		}
		return FAILURE;
	}
	fclose(file);

	// option_output = true, then move to file handling
	if (option_output) {
		// Exit if too many arguments "xti -o chemistry_program file.ext"
		if (argc != 4) {
			return error_fail_message("Wrong number of arguments for output\n");
		}

		// Check for valid file extensions
		if (!check_file_extension(file_name, ".log")) {
			return error_fail_message(
				"The file \"%s\" does not have a valid extension for outputs\n",
				file_name);
		}

		printf("Generate output file\n");
		printf("Input success: \"%s\"\n", file_name);
	}
	// option_input = true, set job_type if specified or default to "sp"
	if (option_input) {
		bool job_memory = false;

		if (argc < 5) {
			// Use default job_type
			job_type = DEFAULT_JOB_TYPE; // Default job_type
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
		if (!check_file_extension(file_name, ".xyz")) {
			return error_fail_message(
				"The file \"%s\" does not have a .xyz extension\n", file_name);
		}

		printf("Generate input file\n");
		printf("Input success: \"%s\"\n", file_name);

		// Pass job_type to Lua OR can execute_lua
		pass_argument_lua(job_type, "JOB_TYPE", "examples/pass_argument.lua");
		// Free memory from job_type if allocate
		if (job_memory) {
			free(job_type);
		}
	}

	// Pass chemistry_program to Lua OR can execute_lua
	pass_argument_lua(chemistry_program, "CHEMISTRY_PROGRAM",
					  "examples/pass_argument.lua");
	// Free memory from chemistry_program
	free(chemistry_program);

	// Pass file_name and path

	// Free memory
	free(option);

	printf("Done!\n");
	return SUCCESS;
}

int error_fail_message(const char *message, ...) {
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	help_prompt();
	return FAILURE;
}
