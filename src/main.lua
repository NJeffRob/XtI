--[[ 
XtI is a program designed to convert xyz files automatically into input
files for a variety of quantum chemistry packages
--]]

-- require other files
require "sh_maker"
require "xyz_converter"
require "help"

-- ASCII character art for nice lil touch :)
print([[
____  ___ __  .___ 
\   \/  //  |_|   |
 \     /\   __\   |
 /     \ |  | |   |
/___/\  \|__| |___|
      \_/          
Type help to get started.
]])

-- Tables to store all the important settings
program_options = {
    ORCA = false,
    Gaussian = false
}

-- change

sh_options = {
    account = "",
    ntasks = "",
    mem_per_cpu = "",
    time = "",
    calc_type = "",
    io_name = "",

    -- ORCA options
    stdenv = "",
    gcc = "",
    openmpi = "",
    orca = ""

    -- Gaussian options

}

orca_input_options = {
    calc_type = "",
    io_name = "",
    functional = "",
    basis_set = "",
    dispersion = "", 
    ntasks = "",
    mem_per_cpu = "",
    charge = "",
    mult = ""
}

gaussian_input_options = {}

-- table of valid commands - saves time in the loop later
list_of_commands = {
    "help",
    "slurm",
    "default",
    "orca",
    "exit"
}

-- main program loop
while input ~= "exit" do
    display = io.write("> ")
    input = io.stdin:read() -- VERY important - do NOT touch this!

    for k, v in pairs(list_of_commands) do
        if input == "help" then
            help()
            break

        elseif input == "slurm" then
            sh_maker_orca()

            print("Submission script successfully generated.")

            io.output(display)
            break

        elseif input == "orca" then
            xyz_converter_orca("", "test", "functional",
            "basis_set", "dispersion", "ntasks", 5,
            "charge", "mult")
    
            print("ORCA input successfully generated.")
            
            io.output(display)
            break

        elseif input == "default" then
            print("make some default settings in the option tables")

            break

        elseif input == "exit" then
            break

        else
            print("Invalid command.")
            break
        end
    end
end
