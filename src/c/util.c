#include "../../include/util.h"
#include <stdio.h>
#include <string.h>

void help_prompt(void) {
  printf("Usage: xti -[options] [program] [job] [file...] \n"
         "Try 'xti -h' for help on getting started. \n");
}

bool is_valid_length(const char *str, size_t min, size_t max) {
  // Prevent segmentation fault
  if (str == NULL) {
    return false;
  }
  size_t len = strlen(str);
  return (len >= min && len <= max);
}

bool match_str(const char *str, const char *static_array[], size_t array_size) {
  size_t num_valid_programs = array_size / sizeof(static_array[0]);
  for (size_t i = 0; i < num_valid_programs; i++) {
    if (strcmp(str, static_array[i]) == 0) {
      return true;
    }
  }
  return false; // If no match is found
}
