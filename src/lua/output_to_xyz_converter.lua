--[[
Each output format is "proprietary" in some way; needs a separate function for each
]]

-- gaussian gives its atomic coordinates via number not atomic symbol :/
local periodic_table = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al",
    "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu",
    "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru",
    "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr",
    "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W",
    "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac",
    "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf",
    "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"
}

-- ORCA: works!
function orca_to_xyz(io_name, calc_type)
    local orca_output_file = assert(io.open(io_name, "r"))

    local found = false

    local initial_coords = {}
    local cleanup_coords = {}
    local xyz_coords = {}

    for line in orca_output_file:lines() do
        if found then
            table.insert(initial_coords, line)
            if line == "" then
                found = false
            end
        elseif line == "CARTESIAN COORDINATES (ANGSTROEM)" then
            found = true 
        end
    end

    orca_output_file:close()

    -- read the coordinates table backwards to get the final geometry
    for i = #initial_coords, 1, -1 do
        table.insert(cleanup_coords, initial_coords[i])
    end

    table.remove(cleanup_coords, 1) -- gets rid of first blank line for following loop

    for k, _ in ipairs(cleanup_coords) do
        table.insert(xyz_coords, cleanup_coords[k])
        if cleanup_coords[k] == "" then
            table.remove(xyz_coords, k)
            table.remove(xyz_coords, k-1)
            break
        elseif string.match(cleanup_coords[k], "%-%-") then
            table.remove(xyz_coords, k)
            break
        end
    end

    local number_of_atoms = 0

    for k, _ in ipairs(xyz_coords) do
        number_of_atoms = k
    end

    -- cleanup the io name to remove the original file extension
    io_name = string.gsub(io_name, "[.][^%.]*$", "")

    -- add table coordinates to new xyz file
    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "w"))

    xyz_from_ouput:write(
        number_of_atoms, "\n",
        "XYZ file generated from XtI.", "\n"
    )

    xyz_from_ouput:close()

    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "a"))

    for _, v in ipairs(xyz_coords) do
        xyz_from_ouput:write(v .. "\n")
    end

    xyz_from_ouput:close()

    print("XYZ coordinate file generated successfully.")
end

-- gamess: works!
function gamess_to_xyz(io_name, calc_type)
    local gamess_output_file = assert(io.open(io_name, "r"))

    local found = false

    local initial_coords = {}
    local cleanup_coords = {}
    local xyz_coords = {}
    local real_xyz_coords = {}

    if calc_type == "opt" then
        for line in gamess_output_file:lines() do
            if found then
                table.insert(initial_coords, line)
                if string.match(line, "INTERNUCLEAR DISTANCES %(ANGS.%)") then
                    found = false
                end
            elseif string.match(line, "COORDINATES OF ALL ATOMS ARE %(ANGS%)") then
                found = true
            end
        end
    else
        for line in gamess_output_file:lines() do
            if found then
                table.insert(initial_coords, line)
                if line == "" then
                    found = false
                end
            elseif string.match(line, "COORDINATES %(BOHR%)") then
                found = true
            end
        end
    end

    gamess_output_file:close()

    -- read the coordinates table backwards to get the final geometry
    for i = #initial_coords, 1, -1 do
        table.insert(cleanup_coords, initial_coords[i])
    end

    if calc_type == "opt" then
        table.remove(cleanup_coords, 1) -- remove first line

        -- stop copying coords after the first set of coords
        for k, _ in ipairs(cleanup_coords) do 
            table.insert(xyz_coords, cleanup_coords[k])
            if string.match(cleanup_coords[k], "%-%-") then
                table.remove(xyz_coords, k)
                break
            end
        end

        -- regular opt coords are in angstroms; idk why it's different for sp/freq
        for k, v in ipairs(xyz_coords) do
            local at0m_symbol = string.sub(v, 1, 2)

            local at0m_coords = string.sub(v, 8, -1)

            local final_xyz_coords = at0m_symbol .. "    " .. at0m_coords

            table.insert(real_xyz_coords, final_xyz_coords)
        end
    else
        table.remove(cleanup_coords, 1) -- gets rid of first blank line for the following loop
        table.remove(cleanup_coords) -- removes last line for the following loop

        -- remove annoying extra floating number; convert the coordinates to angstroms
        for k, v in ipairs(cleanup_coords) do
            local at0m_symbol = string.sub(v, 1, 2)

            local atom_coord_x = tonumber(string.sub(v, 21, 34))
            local atom_coord_y = tonumber(string.sub(v, 41, 53))
            local atom_coord_z = tonumber(string.sub(v, 60, -1))

            local ang_atom_coord_x = atom_coord_x * 0.529177249 
            local ang_atom_coord_y = atom_coord_y * 0.529177249 
            local ang_atom_coord_z = atom_coord_z * 0.529177249

            local final_xyz_coords = at0m_symbol .. "    " .. ang_atom_coord_x .. "    " .. ang_atom_coord_y .. "    " .. ang_atom_coord_z

            table.insert(xyz_coords, final_xyz_coords)
        end
    end

    local number_of_atoms = 0

    for k, _ in ipairs(xyz_coords) do
        number_of_atoms = k
    end

    -- cleanup the io name to remove the original file extension
    io_name = string.gsub(io_name, "[.][^%.]*$", "")

    -- add table coordinates to new xyz file
    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "w"))

    xyz_from_ouput:write(
        number_of_atoms, "\n",
        "XYZ file generated from XtI.", "\n"
    )

    xyz_from_ouput:close()

    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "a"))

    if calc_type == "opt" then
        for _, v in ipairs(real_xyz_coords) do
            xyz_from_ouput:write(v .. "\n")
        end
    else
        for _, v in ipairs(xyz_coords) do
            xyz_from_ouput:write(v .. "\n")
        end
    end

    xyz_from_ouput:close()

    print("XYZ coordinate file generated successfully.")
end

-- fhiaims: works!
function fhiaims_to_xyz(io_name, calc_type)
    local fhiaims_output_file = assert(io.open(io_name, "r"))

    local found = false

    local initial_coords = {}
    local xyz_coords = {}

    -- MODIFY THE ORCA SCRIPT TO MATCH THIS - WAY CLEANER
    for line in fhiaims_output_file:lines() do
        if found then
            if string.match(line, "%-%-") ~= nil then
                found = false
            else
                table.insert(initial_coords, line)
            end
        elseif string.match(line, "Final atomic structure") ~= nil then
            found = true 
        end
    end

    fhiaims_output_file:close()

    table.remove(initial_coords, 1) -- get rid of first line

    for k,v in ipairs(initial_coords) do
        local atom_symbol = string.sub(v, -1, -1)

        local atom_coords = string.sub(v, 17, -2)

        local final_xyz_file = atom_symbol .. atom_coords

        table.insert(xyz_coords, final_xyz_file)
    end

    local number_of_atoms = 0

    for k,_ in ipairs(xyz_coords) do
        number_of_atoms = k
    end

    -- cleanup the io name to remove the original file extension
    io_name = string.gsub(io_name, "[.][^%.]*$", "")

    -- add table coordinates to new xyz file
    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "w"))

    xyz_from_ouput:write(
        number_of_atoms, "\n",
        "XYZ file generated from XtI.", "\n"
    )

    xyz_from_ouput:close()

    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "a"))

    for _, v in ipairs(xyz_coords) do
        xyz_from_ouput:write(v .. "\n")
    end

    xyz_from_ouput:close()

    print("XYZ coordinate file generated successfully.")  
end

-- gaussian: works!
function guassian_to_xyz(io_name, calc_type)
    local gaussian_output_file = assert(io.open(io_name, "r"))

    local found = false

    local initial_coords = {}
    local cleanup_coords = {}
    local xyz_coords = {}

    -- MODIFY THE ORCA SCRIPT TO MATCH THIS - WAY CLEANER
    for line in gaussian_output_file:lines() do
        if found then
            if string.match(line, "Distance") or string.match(line, "Rotational") ~= nil then
                found = false
            else
                table.insert(initial_coords, line)
            end
        elseif string.match(line, "Coordinates %(Angstroms%)") ~= nil then
            found = true 
        end
    end

    gaussian_output_file:close()
    
    -- read the coordinates table backwards to get the final geometry
    for i = #initial_coords, 1, -1 do
        table.insert(cleanup_coords, initial_coords[i])
    end
    
    table.remove(cleanup_coords, 1) -- gets rid of first line

    for k,v in ipairs(cleanup_coords) do
        -- simple string manipulation to get the coords in the correct format
        local atomic_number = string.sub(v, 18, 18)
        
        -- convert atomic number to atomic symbol
        for k,v in ipairs(periodic_table) do
            if tostring(k) == atomic_number then
                atomic_symbol = v -- has to be global var
            end
        end

        local gaussian_coords = string.sub(v, 35, 70)

        local final_coords = atomic_symbol .. gaussian_coords

        table.insert(xyz_coords, final_coords)
        if string.match(cleanup_coords[k], "%-%-") then
            table.remove(xyz_coords, k)
            break
        end
    end

    local number_of_atoms = 0

    for k, _ in ipairs(xyz_coords) do
        number_of_atoms = k
    end

    -- cleanup the io name to remove the original file extension
    io_name = string.gsub(io_name, "[.][^%.]*$", "")

    -- add table coordinates to new xyz file
    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "w"))

    xyz_from_ouput:write(
        number_of_atoms, "\n",
        "XYZ file generated from XtI.", "\n"
    )

    xyz_from_ouput:close()

    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "a"))

    for _, v in ipairs(xyz_coords) do
        xyz_from_ouput:write(v .. "\n")
    end

    xyz_from_ouput:close()

    print("XYZ coordinate file generated successfully.")   
end
