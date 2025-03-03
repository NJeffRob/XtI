#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

// use regex:
// check input -> if input is real try and match it to options using regex

// Regex; need to make sure that the first argument is along the lines of -p/s/i/o/j
int user_matching(const char * user_input) {
    regex_t regex;

    int xti_input_options;

    // need to figure out the proper way to regex: -piosj
    xti_input_options = regcomp(&regex, "", REG_EXTENDED|REG_NOSUB); 

    xti_input_options = regexec(&regex, user_input, 0, NULL, 0);

    if (xti_input_options == 0) {
        printf("hurray! \n");

        regfree(&regex);

        return 0;
    }
    else {
        printf("Usage: xti [options] [file ...] \n"
            "Try 'xti -h' for help on getting started. \n"
            );

        regfree(&regex);

        return 1;
    }
}


int main(int argc, char *argv[]) {
    // if the user doesn't input anything
    if (argc < 2) {
        printf("Usage: xti [options] [file ...] \n"
                "Try 'xti -h' for help on getting started. \n"
                );
        return 1;
    }
    else{
        user_matching(argv[1]);

        //char test_string[] = "test";

        //if (strcmp(argv[1],test_string) == 0) {
         //   printf(argv[1]);
        //    printf("\n");
        //    printf(test_string);
        //    printf("\n");
       // }
        return 0;
    }
}