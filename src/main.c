// The plan:
// 1. main tool/options using C, i.e. all argument parsing and calling of functions 
// is WITHIN THIS PROGRAM
// 2. all functions/functionality using Lua embedded withing C
// 3. ???? hope it works I guess

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    char test_string[] = "test";

    //printf("\n");
    if (strcmp(argv[1],test_string) == 0) {
        printf(argv[1]);
        printf("\n");
        printf(test_string);
        printf("\n");
    }


	return 0;
}