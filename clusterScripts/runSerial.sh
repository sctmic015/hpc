#!/bin/sh
#SBATCH --partition=curie
#SBATCH --time=00:10:00
#SBATCH --nodes=1 --ntasks=1
#SBATCH --job-name="SerialFilter"
#SBATCH --mail-user=sctmic015@myuct.ac.za
#SBATCH --mail-type=ALL

/home/sctmic015/hpc/serial /home/sctmic015/hpc/data /home/sctmic015/hpc/dataOut 3