#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "lua.h" 
#include "lauxlib.h" 
#include "lualib.h"

#define SUCCESS 0
#define FAILURE 1

// the following structs are for program and job options
struct Programs {
    char abinit[7];
    char atomicrex[10];
    char atompaw[8];
    char beef[5];
    char cantera[8];
    char cfour[6];
    char CheMP2[7];
    char coordgenlibs[13];
    char cp2k[5];
    char cpmd[5];
    char crest[6];
    char ctffind[8];
    char dalton[7];
    char gamess[7];
    char gromacs[8];
    char nwchem[7];
    char molcas[7];
    char plumed[7];
    char psi4[5];
    char quantumespresso[16];
    char rosetta[8];
    char xtb[4];
    char orca[5];
    char gaussian[9];
    char fhiaims[8];
    char qchem[6];
};

struct Job_types {
    char opt[30];
    char freq[30];
    char sp[30];
};


// Regex; need to make sure that the first argument is along the lines of -p/s/i/o/j
int input_option_checking(const char * user_input) {
    regex_t regex;

    int xti_input_options;

    /* below is the best I could come up with 
    KNOWN ISSUE: if the user double inputs an option, it is accepted, e.g. -pp */
    const char *input_options = "^-[psioj]{1,6}";

    xti_input_options = regcomp(&regex, input_options, REG_EXTENDED|REG_NOSUB); 

    xti_input_options = regexec(&regex, user_input, 0, NULL, 0);

    if (xti_input_options == SUCCESS) {
        regfree(&regex);

        return SUCCESS;
    }
    else {
        regfree(&regex);

        return FAILURE;
    }
}


int main(int argc, char *argv[]) {
    char help[10] = "-h";

    // if the user doesn't input anything
    if (argc < 2) {
        printf("Usage: xti [options] [file ...] \n"
                "Try 'xti -h' for help on getting started. \n"
                );
        
        return FAILURE;
    }
    else{
        /* this else section has to initialize a Lua API call, i.e. all the following code
            has to be able to run lua
           if the user returns an appropriate input option, check the next input
           if the next input is appropriate for the input options listed, check the next input 
           if the next input is a name of a file in the directory, do stuff */
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);

        // prints the help message from the lua file
        // NOTE: LUA FILE IS NOT COMPILED WITH THE REST OF THE PROGRAM
        if (strcmp(argv[1],help) == 0) {
            if (luaL_dofile(L, "src/help.lua") == LUA_OK) {
                lua_pop(L, lua_gettop(L));
            }
        }   
        else if (input_option_checking(argv[1]) == SUCCESS) {
            printf("%s\n", argv[1]);
        }
        else {
            printf("Usage: xti [options] [file ...] \n"
                "Try 'xti -h' for help on getting started. \n"
                );
        }
        /*
        int i;
        for (i = 1; i < argc; i++) {
            input_option_checking(argv[i]);
        }
        */
        lua_close(L);

        return SUCCESS;
    }
}
