#!/bin/sh
#SBATCH --partition=curie
#SBATCH --time=00:01:00
#SBATCH --nodes=2 --ntasks=32 --ntasks-per-node=16
#SBATCH --job-name="HelloWorld"
#SBATCH --mail-user=michelle.kuttel@uct.ac.za
#SBATCH --mail-type=ALL
module load mpi/openmpi-4.0.1
mpirun /home/hpc48/helloWorld > run.out
