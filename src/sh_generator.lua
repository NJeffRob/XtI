-- modify these as is your wont
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
]]
local function sh_generator (user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)
    local sh = io.open(io_name .. ".sh", "w")

    io.output(sh)

    io.write(
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
       -- "module load " .. program, "\n", -- specific to a program
       -- program .. " " .. io_name .. ".in > " .. io_name .. ".out" -- specific to a program
    )
end

function orca_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local orca_sh = io.open(io_name .. ".sh", "a")

    io.write(
        "orca stuff"
    )

    io.close(orca_sh)

    print(".sh script generated successfully.")
end

function gaussian_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local gaussian_sh = io.open(io_name .. ".sh", "a")

    io.write(
        "write stuff"
    )

    io.close(gaussian_sh)

    print(".sh script generated successfully.")
end

function abinit_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local abinit_sh = io.open(io_name .. ".sh", "a")

    io.write(
        "write stuff"
    )

    io.close(abinit_sh)

    print(".sh script generated successfully.")
end


function template_sh(io_name, calc_type)
    sh_generator(user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)

    local template_sh = io.open(io_name .. ".sh", "a")

    io.write(
        "write stuff"
    )

    io.close(template_sh)

    print(".sh script generated successfully.")
end
