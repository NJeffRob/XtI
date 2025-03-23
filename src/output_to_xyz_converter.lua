--[[
Each output format is proprietary in some way; needs a separate function for each
]]

-- ORCA: works!
function orca_to_xyz(io_name, calc_type)
    local orca_opt_output_file = assert(io.open(io_name, "r"))

    local found = false

    local initial_coords = {}
    local cleanup_coords = {}
    local xyz_coords = {}

    for line in orca_opt_output_file:lines() do
        if found then
            table.insert(initial_coords, line)
            if line == "" then
                found = false
            end
        elseif line == "CARTESIAN COORDINATES (ANGSTROEM)" then
            found = true 
        end
    end

    orca_opt_output_file:close()

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
        "XYZ file generated from XtI", "\n"
    )

    xyz_from_ouput:close()

    local xyz_from_ouput = assert(io.open(io_name .. "-" .. calc_type .. ".xyz", "a"))

    for _, v in ipairs(xyz_coords) do
        xyz_from_ouput:write(v .. "\n")
    end

    xyz_from_ouput:close()

    print("XYZ coordinate file generated successfully.")
end

-- abinit: in progress
function abinit_to_xyz()
    
end

-- gamess: in progress
function gamess_to_xyz()
    
end

-- qe: in progress
function qe_to_xyz()
    
end

-- fhiaims: in progress
function fhiaims_to_xyz()
    
end

-- qchem: in progress
function qchem_to_xyz()
    
end

-- siesta: in progress
function siesta_to_xyz()
    
end

-- vasp: in progress
function vasp_to_xyz()
    
end

-- castep: in progress
function castep_to_xyz()
    
end

-- gaussian: in progress (piece of shit program)
function guassian_to_xyz()

end

