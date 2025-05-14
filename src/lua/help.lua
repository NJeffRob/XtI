-- function to print the help options; need to fill this with actually relevant info lol
function help()
    print([[
Usage:
xti [options] [file ...]

Options
-i                    Generate an input file from a .xyz coordinate file.
-o                    Generate a .xyz coordinate file from an output file.
-s                    Generate a .sh submission script.

-h                    Print this help message.

Usage examples:
1. Generate an ORCA input file (.inp) and submission script (.sh) from a .xyz coordinate file of water:
input: xti -is orca water.xyz
output: water.inp, water.sh

2. Generate a .xyz coordinate file from a Gaussian output (.log) file of water:
input: xti -o gaussian water.log
output: water.xyz

3. Generate a submission script (.sh) for FHI-aims for a file called water:
input: xti -s fhiaims water
output: water.sh

4. Generate a Gaussian frequency calculation input (.gjf) and submission script (.sh) from a .xyz coordinate file of water:
input: xti -si gaussian freq water.xyz
output: water-freq.gjf, water-freq.sh
]]

)
end

help()
