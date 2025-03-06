#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "lua.h" 
#include "lauxlib.h" 
#include "lualib.h"

#define GOOD 0
#define BAD 1

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


/* use regex:
check input -> if input is real try and match it to options using regex */

// Regex; need to make sure that the first argument is along the lines of -p/s/i/o/j
int input_option_checking(const char * user_input) {
    regex_t regex;

    int xti_input_options;

    /* below is the best I could come up with 
    KNOWN ISSUE: if the user double inputs an option, it is accepted, e.g. -pp */
    const char *input_options = "^-[psiojh]{1,6}";

    xti_input_options = regcomp(&regex, input_options, REG_EXTENDED|REG_NOSUB); 

    xti_input_options = regexec(&regex, user_input, 0, NULL, 0);

    if (xti_input_options == 0) {
        printf("hurray! \n");

        regfree(&regex);

        return GOOD;
    }
    else {
        printf("Usage: xti [options] [file ...] \n"
            "Try 'xti -h' for help on getting started. \n"
            );

        regfree(&regex);

        return BAD;
    }
}


int main(int argc, char *argv[]) {
    char help[10] = "-h";

    // if the user doesn't input anything
    if (argc < 2) {
        printf("Usage: xti [options] [file ...] \n"
                "Try 'xti -h' for help on getting started. \n"
                );
        
        return BAD;
    }
    else{
        /* this else section has to initialize a Lua API call, i.e. all the following code
            has to be able to run lua
           if the user returns an appropriate input option, check the next line
           if the next line is appropriate for the input options listed, check the next line 
           if the next line is a name of a file in the directory, do stuff */
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);

        if (strcmp(argv[1],help) == 0) {
            if (luaL_dofile(L, "help.lua") == LUA_OK) {
                lua_pop(L, lua_gettop(L));
            }
        }

        /*
        int i;
        for (i = 1; i < argc; i++) {
            input_option_checking(argv[i]);
        }
        */
        lua_close(L);

        return GOOD;
    }
}
