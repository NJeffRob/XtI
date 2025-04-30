#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stddef.h>

bool is_valid_length(const char *str, size_t min, size_t max);
bool match_str(const char *str, const char *static_array[], size_t array_size);

#endif
