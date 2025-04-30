#include "../../include/option_handler.h"
#include "../../include/lua_util.h"
#include "../../include/util.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_valid_option(char c) {
  // Check if the character is 's', 'i', 'o', or 'j'
  return (c == 's' || c == 'i' || c == 'o' || c == 'j');
}

bool is_duplicate_option(const char *str) {
  // Check for duplicate characters
  for (int i = 1; str[i] != '\0'; i++) {
    for (int j = i + 1; str[j] != '\0'; j++) {
      if (str[i] == str[j]) {
        return true;
      }
    }
  }
  return false;
}


