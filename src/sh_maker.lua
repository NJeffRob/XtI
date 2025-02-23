-- functions to make relevant sh scripts for high performance cluster submission
function sh_maker_orca(account, ntasks, mem_per_cpu, time, calc_type, stdenv, gcc, openmpi, orca, io_name)
    sh_file = io.open(io_name .. ".sh", "w")

    io.output(sh_file)

    io.write(
        "#!/bin/bash", "\n",
        "#SBATCH --account=" .. account, "\n",
        "#SBATCH --nodes=1", "\n",
        "#SBATCH --ntasks=" .. ntasks, "\n",
        "#SBATCH --mem-per-cpu=" .. mem_per_cpu .. "G", "\n",
        "#SBATCH --time=" .. time, "\n",
        "#SBATCH --output=%j-" .. calc_type, "\n",
        "", "\n",
        "module purge", "\n",
        "module load StdEnv/" .. stdenv .. "  gcc/" .. gcc .. "  openmpi/" .. openmpi, "\n",
        "module load orca/" .. orca, "\n",
        "$EBROOTORCA/orca " .. io_name .. ".inp > " .. io_name .. ".out"
    )
end
