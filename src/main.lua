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

orca_input_options = {}

gaussian_input_options = {}

-- main program loop
while Input ~= "exit" do
    Display = io.write("> ")
    Input = io.stdin:read() -- VERY important - do NOT touch this!

    if Input == "help" then
        help()

    elseif Input == "sh" then
        sh_maker_orca("account", "ntasks",
        "mem_per_cpu", "time", "calc_type",
        "stdenv", "gcc", "openmpi", "orca", "io_name")

        io.output(Display)

    elseif Input == "orca" then
        xyz_converter("", "test", "functional",
        "basis_set", "dispersion", "ntasks", 5,
        "charge", "mult")
        
        io.output(Display)
    end
end
