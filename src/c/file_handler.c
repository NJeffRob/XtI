#include "../../include/file_handler.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int file_xyz_extension(const char *file_name) {
  const char *extension = strrchr(file_name, '.');
  return (extension != NULL && strcmp(extension, ".xyz") == 0);
}
