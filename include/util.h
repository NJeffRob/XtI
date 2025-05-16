#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help_prompt(void);
char *iso_strdup(const char *s);
char *convert_to_lower(char *str);
bool is_valid_length(const char *str, size_t min, size_t max);
bool match_str(const char *str, const char *static_array[], size_t array_size);

#endif
