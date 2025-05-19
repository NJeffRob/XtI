#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdlib.h>
#include <string.h>

#include "util.h"

int check_file_extension(const char *file_name, const char *check_extension);
char *remove_file_extension(const char *file_name);

#endif
