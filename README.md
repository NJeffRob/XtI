# XYZ to Input (XtI)
This is a basic Lua script to turn xyz coordinate files into input files for the following electronic structure programs:
- bleh
- bla
- bloo

(update the readme when the program is done)

# To dos
## Lawrence
- Get user input in the form of the following: xti -option program jobtype file
- For the above: programs list can be found in the "Programs" struct in main.c; options list can be found in "help.lua"; jobtype can be found in the "Job_types" structure in main.c; and file should be a valid file
- Once all user input is correctly checked, pass the argument to the relevant Lua function in either the sh, xyz_to_input, or output_to_xyz Lua files (I can help with implementing this; see: https://lucasklassmann.com/blog/2019-02-02-embedding-lua-in-c/ for a great guide)
- *maybe*: customize the API to be more modular (this might be nightmarish, so don't worry about it til WAY later)
- **When code is done**: create a makefile for easy compilation

## Nick
- Add in more program options to the Lua files for input, output and sh generation
- Write a proper readme/better documentation in general
- Finish writing the paper draft/get supervisor to approve and submit it for publication

### Requirements
The raw code for this project was written in Lua 5.4.7, without the use of other dependencies. Executables can be found... 

It is recommended that XtI be downloaded as an executable and added to the users path. To do this... You can then use the program from whatever directory you like.

### Use
XtI comes with the following options:
- bleh
- blagh
- bloo

into the command line. This will prompt the user with some questions pertaining to their [ORCA](https://pubs.aip.org/aip/jcp/article/152/22/224108/1061982/The-ORCA-quantum-chemistry-program-package) job, and then ask if the user wants to set custom parameters or use some defaults. **Note: your molecule name has to be the same as what you've titled your xyz file.**

The default parameters are as follows:
| Parameter   | Default     |
| ----------- | ----------- |
| Functional  | PBE0        |
| Basis Set   | def2-SVP    |
| Dispersion  | D3BJ        |
| cpus        | 4           |
| memory      | 2GB         |
| charge      | 0           |
| mult        | 1           |

These defaults were chosen with molecular geometries in mind (sorry materials folks). You can of course specify custom inputs instead should you need something more specific, or modify the defaults as you wish. **Note that for both the default and the custom version, the user will have to specify their DRAC account.**

Custom parameters are anything the user specifies. **If your input contains gibberish, the output will be gibberish. There are no checks that the user has input something sensible. Always double check your inputs.**

Whether the user selects custom (c) or default (d), the script will output two files: a submission shell script for the [Digital Research Alliance of Canada](https://docs.alliancecan.ca/wiki/Technical_documentation), and a .inp file that contains relevant ORCA code. It is encouraged that you double check that these files are both correct before you submit further jobs.

### Testing
Integration tests on C input handling with the [Check](https://libcheck.github.io/check/).

Dependencies are check, and pkg-config, installed with `sudo apt install check pkg-config`

Run `make` and `make test`

The file `scripts/generate_test.py` will automatically create the boilerplate for a C test and update the Makefile. The script can be ran with `python3 scripts/generate_test.py <test_name>.c`

