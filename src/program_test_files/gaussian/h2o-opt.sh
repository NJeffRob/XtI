#!/bin/bash
#SBATCH --job-name=h2o
#SBATCH --account=def-ipaci
#SBATCH --output=%j-opt.log
#SBATCH --error=%j-opt.err
#SBATCH --cpus-per-task=4
#SBATCH --mem=8G
#SBATCH --time=00-00:10

module purge
module load gaussian/g16.c01
g16 < h2o-opt.gjf >& h2o-opt.log
