#!/bin/bash
#SBATCH --job-name=src/program_test_files/h2
#SBATCH --account=your_account
#SBATCH --output=%j-sp.log
#SBATCH --error=%j-sp.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=your_email
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4
#SBATCH --mem-per-cpu=1G
#SBATCH --time=01-00:00

module purge
module load gamess-us/20170420-R1
export SLURM_CPUS_PER_TASK
rungms src/program_test_files/h2-sp.inp &> src/program_test_files/h2-sp.out