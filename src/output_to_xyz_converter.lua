-- currently this is specific to ORCA

local orca_opt_output_file = assert(io.open("Ar-B-diOH-sp-ORCA.out", "r"))

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
    end
end

local number_of_atoms = 0

for k, v in ipairs(xyz_coords) do
    number_of_atoms = k
end

-- add table coordinates to new xyz file
local xyz_from_ouput = assert(io.open("grr" .. ".xyz", "w"))

xyz_from_ouput:write(
    number_of_atoms, "\n",
    "XYZ file generated from XtI", "\n"
)

xyz_from_ouput:close()

local xyz_from_ouput = assert(io.open("grr" .. ".xyz", "a"))

for _, v in ipairs(xyz_coords) do
    xyz_from_ouput:write(v .. "\n")
end

xyz_from_ouput:close()

-- this all works for ORCA opts!
-- does NOT work for freq/sp! What is happening!!!

--[[
open output file
make new empty xyz file
find xyz coordinates
yoink xyz coordinates into a table2
find out max length of table (its highest key)
copy to new xyz file in the correct format, i.e. # atoms, text, coordinates
]]