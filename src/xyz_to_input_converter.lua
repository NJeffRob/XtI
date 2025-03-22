local functional <const> = "PBE0"
local basis_set <const> = "def2-SVP"
local sp_basis_set <const> = "def2-TZVPP"
local dispersion <const> = "D3BJ"
local ntasks <const> = "1"
local mem_per_cpu <const> = "1"
local charge <const> = "0"
local mult <const> = "1"

-- function to extract the xyz coordinates from the .xyz file
local function xyz_extractor(io_name)
    -- this returns the path separator, which lets me copy the .xyz file based on the OS
    operating_system = package.config:sub(1,1) 
    if operating_system == "\\" then
        os.execute("copy " .. io_name .. ".xyz " .. io_name)
    elseif operating_system == "/" then
        os.execute("cp " .. io_name .. ".xyz " .. io_name)
    end

    local atomic_coordinates = {}

    -- extract coordinates
    local xyz_file = assert(io.open(io_name, "r"))

    for line in xyz_file:lines() do
        table.insert(atomic_coordinates, line)
    end

    local count = 1
    while count < 3 do
        table.remove(atomic_coordinates, 1)
        count = count + 1
    end

    -- put coordinates into the new file
    local output_coordinates = assert(io.open(io_name, "w"))

    io.input(output_coordinates)

    io.output(output_coordinates)

    for _, line in ipairs(atomic_coordinates) do
        output_coordinates:write(line)
        output_coordinates:write("\n")
    end

    output_coordinates:close()
end

-- append the xyz coordinates to the appropriate input files
function xyz_to_orca(io_name, calc_type, functional, basis_set, sp_basis_set, dispersion, ntasks, mem_per_cpu, charge, mult)
    xyz_extractor(io_name)

    local orca_inp_file = assert(io.open(io_name .. ".inp", "w"))

    if calc_type == "sp" then
        orca_inp_file:write(
            "! " .. functional .. " ".. sp_basis_set .. " " .. dispersion .. " printbasis", "\n"
        )
    else
        orca_inp_file:write(
        "! " .. functional .. " ".. basis_set .. " " .. dispersion .. " printbasis", "\n",
        "! " .. calc_type, "\n"
        )
    end

    orca_inp_file:close()
    
    local coordinates = assert(io.open(io_name, "r"))
    local orca_inp_file = assert(io.open(io_name .. ".inp", "a"))

    orca_inp_file:write(
        "", "\n",
        "%pal", "\n",
        "  nprocs " .. ntasks, "\n",
        "end", "\n",
        "", "\n",
        "%maxcore " .. mem_per_cpu*750, "\n", -- 75% of memory
        "", "\n",
        "* xyz " .. charge .. " " .. mult,
        "\n"
    )

    for line in coordinates:lines() do
        orca_inp_file:write(line .. "\n")
    end

    orca_inp_file:write("*")

    orca_inp_file:close()
    coordinates:close()

    os.remove(io_name) -- get rid of the temp coordinates

    print("ORCA input file generated successfully.")
end
