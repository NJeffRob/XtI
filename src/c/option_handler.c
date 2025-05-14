#include "../../include/option_handler.h"
#include <stdbool.h>

bool is_valid_option(char c) {
	// Check if the option flag is -i for input, or -o for output
	return (c == 'i' || c == 'o' || c == 's');
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
