#!/bin/bash
#SBATCH --job-name=src/program_test_files/h2.xyz
#SBATCH --account=your_account
#SBATCH --output=%j-freq.log
#SBATCH --error=%j-freq.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=your_email
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4
#SBATCH --mem-per-cpu=1G
#SBATCH --time=01-00:00

module purge
module load StdEnv/2023 gcc/12.3 openmpi/4.1.5
module load 6.0.1
$EBROOTORCA/orca src/program_test_files/h2.xyz-freq.inp > src/program_test_files/h2.xyz-freq.out
