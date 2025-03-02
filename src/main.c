#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

struct options {

};

// use regex:
// check input -> if input is real try and match it to options using regex

int main(int argc, char *argv[]) {
    // if the user doesn't input anything
    if (argc < 2) {
        printf("Usage: xti [options] [file ...] \n"
                "Try 'xti -h' for help on getting started. \n"
                );
        return 1;
    }
    // need to make sure that the first argument is along the lines of -p/s/i/o
    else if (argv[1] != "oop") {
        printf("Usage: xti [options] [file ...] \n"
            "Try 'xti -h' for help on getting started. \n"
            );
        return 1;
    }
    else{
        char test_string[] = "test";

        if (strcmp(argv[1],test_string) == 0) {
            printf(argv[1]);
            printf("\n");
            printf(test_string);
            printf("\n");
        }
        return 0;
    }
}