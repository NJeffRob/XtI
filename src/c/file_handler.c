#include "../../include/file_handler.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int check_file_extension(const char *file_name, const char *check_extension) {
	const char *extension = strrchr(file_name, '.');
	return (extension != NULL && strcmp(extension, check_extension) == 0);
}
