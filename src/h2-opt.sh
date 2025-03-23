#!/bin/bash
#SBATCH --job-name=h2
#SBATCH --account=your_account
#SBATCH --output=%j-opt.log
#SBATCH --error=%j-opt.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=your_email
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4
#SBATCH --mem-per-cpu=1G
#SBATCH --time=01-00:00

module purge
module load gaussian/g16.c01
G16 h2-opt.gjf > h2-opt.log