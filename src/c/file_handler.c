#include "../../include/file_handler.h"
#include "../../include/util.h"

int check_file_extension(const char *file_name, const char *check_extension) {
	const char *extension = strrchr(file_name, '.');
	return (extension != NULL && strcmp(extension, check_extension) == 0);
}

char *remove_file_extension(const char *file_name) {
	char *no_ext = iso_strdup(file_name);
	if (!no_ext)
		return NULL;

	char *last_dot = strrchr(no_ext, '.');
	char *last_slash = strrchr(no_ext, '/');

	// Only remove the extension if the dot comes after the last slash
	if (last_dot && (!last_slash || last_dot > last_slash)) {
		*last_dot = '\0';
	}

	return no_ext;
}
