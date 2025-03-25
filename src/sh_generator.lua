-- modify these as you wish
-- NOTE: do NOT add 'io_name' or 'calc_type' to this list of consts
local user_mail <const> = "your_email"
local account <const> = "your_account"
local ntasks <const> = "4"
local nodes <const> = "1"
local cpus_per_task <const> = "1"
local mem <const> = "4"
local mem_per_cpu <const> = "1"
local time <const> = "01-00:00"

--[[
Main function that writes the bulk of the sh file; other functions append onto this.
To make your own functions, a template has been provided at the bottom.
Note that the provided templates are structured for DRAC clusters, but can be easily modified.
]]

--[[ 
Is string interpolation better here? Or worse?
Come back to this once you've figured out more of the C code
]]
local function sh_generator (user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)
    local sh = assert(io.open(io_name .. "-" .. calc_type .. ".sh", "w"))

    sh:write(
        "#!/bin/bash", "\n",
        "#SBATCH --job-name=" .. io_name, "\n",
        "#SBATCH --account=" .. account, "\n",
        "#SBATCH --output=%j-" .. calc_type .. ".log", "\n",
        "#SBATCH --error=%j-" .. calc_type .. ".err", "\n",
        "#SBATCH --mail-type=END,FAIL", "\n",
        "#SBATCH --mail-user=" .. user_mail, "\n",
        "#SBATCH --nodes=" .. nodes, "\n",
        "#SBATCH --ntasks=" .. ntasks, "\n",
        "#SBATCH --cpus-per-task=" .. cpus_per_task, "\n",
        "#SBATCH --mem=" .. mem, "\n",
        "#SBATCH --mem-per-cpu=" .. mem_per_cpu .. "G", "\n",
        "#SBATCH --time=" .. time, "\n",
        "", "\n",
        "module purge", "\n" --,
       -- "module load " .. program, "\n", specific to a program
       -- program .. " " .. io_name .. ".in > " .. io_name .. ".out" specific to a program
    )

    sh:close()
end

-- ORCA: works!
function orca_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local orca_sh_file = io.open(io_name .. "-" .. calc_type .. ".sh", "a")

    io.write(
        "module load StdEnv/2023 gcc/12.3 openmpi/4.1.5", "\n",
        "module load 6.0.1", "\n",
        "$EBROOTORCA/orca " .. io_name .. "-" .. calc_type .. ".inp" .. " > " .. io_name .. "-" .. calc_type .. ".out", "\n"
    )

    io.close(orca_sh_file)

    print(".sh script generated successfully.")
end

-- Gaussian: works!
function gaussian_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local gaussian_sh_file = assert(io.open(io_name .. "-" .. calc_type .. ".sh", "a"))

    gaussian_sh_file:write(
        "module load gaussian/g16.c01", "\n",
        "g16 < " ..  io_name .. "-" .. calc_type .. ".gjf" .. " >& " .. io_name .. "-" .. calc_type .. ".log"
    )

    gaussian_sh_file:close()

    print(".sh script generated successfully.")
end

-- Abinit: in progress
function abinit_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local abinit_sh = io.open(io_name .. "-" .. calc_type .. ".sh", "a")

    io.write(
        "write stuff"
    )

    io.close(abinit_sh)

    print(".sh script generated successfully.")
end


function template_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local template_sh = io.open(io_name .. "-" .. calc_type .. ".sh", "a")

    io.write(
        "write stuff"
    )

    io.close(template_sh)

    print(".sh script generated successfully.")
end

gaussian_sh("water", "opt")
