#include <regex.h>
#include <stdio.h>

void print_result(int value)
{
 
    // If pattern found
    if (value == 0) {
        printf("Pattern found.\n");
    }
 
    // If pattern not found
    else if (value == REG_NOMATCH) {
        printf("Pattern not found.\n");
    }
 
    // If error occurred during Pattern
    // matching
    else {
        printf("An error occurred.\n");
    }
}

int main() {
  regex_t reegex;

  int value;

  //value = regcomp(&reegex, "^-(?!.*(.).*\1)[psiojh]+(?: (?!.*(.).*\1)[psiojh]+)*$", 0);
  value = regcomp(&reegex, "^-(?!.*([psioj]).*\1)[psioj]+$", 0);

  if (value == 0) {
    printf("Compile success\n");
  }
  else {
    printf("Compile failed\n");
  }

  char testString[] = "-psi";
  value = regexec(&reegex, testString, 0, NULL, 0);
  print_result(value);

  return 0;
}

