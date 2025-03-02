-- function to print the help options; need to fill this with actually relevant info lol
function help()
    print([[
Usage:
xti [options] [file ...]

Options
-p, --program         Specify the electronic structure program for input, output or submission script.
-i, --input           Generate an input file from a .xyz coordinate file.
-o, --output          Generate a .xyz coordinate file from an output file.
-s, --sh              Generate a .sh submission script.

-h, --help            Print this help message.

Usage examples:
1. Generate an ORCA input file (.inp) and submission script (.sh) from a .xyz coordinate file of water:
input: xti -pis orca water.xyz
output: water.inp, water.sh

2. Generate a .xyz coordinate file from a Gaussian output (.log) file of water:
input: xti -po gaussian water.log
output: water.xyz

3. Generate a submission script (.sh) for FHI-AIMS for a file called water:
input: xti -ps fhiaims water
output: water.sh

    ]])
end

help()