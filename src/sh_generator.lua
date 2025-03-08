-- modify these as is your wont
-- NOTE: do NOT add 'io_name' to this list of consts
local user_mail <const> = "different"
local account <const> = "different"

--[[
Main function that writes the bulk of the sh file; other functions append onto this.
To make your own functions, a template has been provided at the bottom.
]]
function sh_generator (user_mail, account, ntasks, nodes, cpus_per_task, mem, mem_per_cpu, time, calc_type, io_name)
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

function orca_sh(io_name)
    sh_generator(user_mail, account, "test", "test", "test", "test", "test", "test", "test", "test")

    local orca_sh = io.open(io_name .. ".sh", "a")

    io.write(
        "orca stuff"
    )

    io.close(orca_sh)

    print(".sh script generated successfully.")
end


function template_sh(io_name)
    sh_generator(user_mail, account, "test", "test", "test", "test", "test", "test", "test", "test")

    local template_sh = io.open(io_name .. ".sh", "a")

    io.write(
        "write stuff"
    )

    io.close(template_sh)

    print(".sh script generated successfully.")
end
