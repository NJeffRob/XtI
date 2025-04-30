#include <stdbool.h> // don't forget to include this lib
#include <stdio.h>
#include <string.h>

// Compile with gcc -o xti input.c
// Call with ./xti [options]

#define SUCCESS 0
#define FAILURE 1

bool isValidOption(char c);
bool isDuplicateOption(char *str);
void storeOption(char option);

int main(int argc, char *argv[]) {
  // First character of the argument is '-'
  if (argv[1][0] != '-') {
    printf("No options specified\n"); // throw help message at the end of each
    return FAILURE;
  }
  // Argument is between 2 to 5 characters
  else if (strlen(argv[1]) < 2 || strlen(argv[1]) > 5) {
    printf("Invalid number of arguments provided: %lu\n", strlen(argv[1]));
    return FAILURE;
  }
  // Argument contains only the characters 's', 'i', 'o', or 'j'
  for (int i = 1; argv[1][i] != '\0'; i++) {
    if (!isValidOption(argv[1][i])) {
      printf("Invalid options '%c'\n", argv[1][i]);
      return FAILURE;
    }
  }
  // No duplicate options
  if (isDuplicateOption(argv[1])) {
    printf("Duplicate options are not allowed\n");
    return FAILURE;
  }

  // Store valid options
  for (int i = 1; argv[1][i] != '\0'; i++) {
    storeOption(argv[1][i]);
  }

  return SUCCESS;
}

bool isValidOption(char c) {
  // Check if the character is 's', 'i', 'o', or 'j'
  return (c == 's' || c == 'i' || c == 'o' || c == 'j');
}

bool isDuplicateOption(char *str) {
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

void storeOption(char option) {
  // Call Lua based on option
  switch (option) {
  case 's':
    printf("Processing option 's'\n");
    break;
  case 'i':
    printf("Processing option 'i'\n");
    break;
  case 'o':
    printf("Processing option 'o'\n");
    break;
  case 'j':
    printf("Processing option 'j'\n");
    break;
  default:
    break;
  }
}
