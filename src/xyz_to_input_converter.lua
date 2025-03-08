local functional <const> = "PBE0"
local basis_set <const> = "def2-SVP"
local dispersion <const> = "D3BJ"
local ntasks <const> = "1"
local mem_per_cpu <const> = "1"
local charge <const> = "0"
local mult <const> = "1"

--[[
Want: function to extract the .xyz coordinates from the .xyz file
then: append those coordinates to the appropriate input file
]]

-- function to extract the xyz coordinates from the .xyz file
function xyz_extractor(io_name)
    -- Read and copy the xyz coordinates of the input
    operating_system = package.config:sub(1,1) -- this returns the path separator, which informs me of the OS
    if operating_system == "\\" then
        os.execute("copy " .. io_name .. ".xyz " .. io_name)
    elseif operating_system == "/" then
        os.execute("cp " .. io_name .. ".xyz " .. io_name)
    end

    local xyz_file = assert(io.open(io_name, "r"))

    xyz_file = io.input()

    local atomic_coordinates = {}

    for line in io.lines() do
        table.insert(atomic_coordinates, line)
    end

    remove_line_1 = table.remove(atomic_coordinates, 1)
    remove_line_1 = table.remove(atomic_coordinates, 1)

    print(atomic_coordinates)

    assert(io.open(io_name, "a"))

    io.output(xyz_file)

    for _, line in ipairs(atomic_coordinates) do
        xyz_file:write(line)
        xyz_file:write("\n")
    end
end


-- functions to turn xyz files into relevant input files for electronic structure programs
function xyz_to_input_file(calc_type, io_name, functional, basis_set, dispersion, ntasks, mem_per_cpu, charge, mult)
    -- Read and copy the xyz coordinates of the input
    operating_system = package.config:sub(1,1) -- this returns the path separator, which informs me of the OS
    if operating_system == "\\" then
        os.execute("copy " .. io_name .. ".xyz " .. io_name .. ".inp")
    elseif operating_system == "/" then
        os.execute("cp " .. io_name .. ".xyz " .. io_name .. ".inp")
    end

    if calc_type == "sp" then
        calc_type = ""
    end

    -- Creates a .inp file using the above coordinates    
    inp_file = io.open(io_name .. ".inp", "r")

    atomic_coordinates = {}

    io.input(inp_file)

    for line in io.lines() do
        table.insert(atomic_coordinates, line)
    end

    -- iterating over a table was weirdly hard? These two lines remove the top two lines the atomic coordinates
    remove_line_1 = table.remove(atomic_coordinates, 1)
    remove_line_1 = table.remove(atomic_coordinates, 1)

    table.insert(atomic_coordinates, "*")

    if calc_type == "" then
        inp_file = io.open(io_name .. ".inp", "w")

        io.output(inp_file)

        io.write(
            "# comment", "\n",
            "! " .. functional .. " ".. basis_set .. " " .. dispersion .. " printbasis", "\n",
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

        io.close(inp_file)

        inp_file = io.open(io_name .. ".inp", "a")

        for _, line in ipairs(atomic_coordinates) do
            inp_file:write(line)
            inp_file:write("\n")
        end
    
        io.close(inp_file)

    else
        inp_file = io.open(io_name .. ".inp", "w")

        io.output(inp_file)

        io.write(
            "# comment", "\n",
            "! " .. functional .. " ".. basis_set .. " " .. dispersion .. " printbasis", "\n",
            "! " .. calc_type, "\n",
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
        
        io.close(inp_file)

        inp_file = io.open(io_name .. ".inp", "a")

        for _, line in ipairs(atomic_coordinates) do
            inp_file:write(line)
            inp_file:write("\n")
        end

        io.close(inp_file)
    end
end

xyz_extractor("water")
