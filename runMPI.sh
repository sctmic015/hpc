#!/bin/sh
#SBATCH --partition=curie
#SBATCH --time=12:00:00
#SBATCH --nodes=6 --ntasks=40 --ntasks-per-node=7
#SBATCH --job-name="MPI40_6"
#SBATCH --mail-user=sctmic015@myuct.ac.za
#SBATCH --mail-type=ALL
export OMP_NUM_THREADS=$SLURM_TASKS_PER_NODE
module load mpi/openmpi-4.0.1

filterSizes=(3 9 15)

for filterSize in "${filterSizes[@]}"
do
    for iteration in {1..2}
    do
        eval "mpirun ./bin/mpiFilter data/small10 out $filterSize >> time/MPISmall40_6_$filterSize"
        eval "mpirun ./bin/mpiFilter data/medium30 out $filterSize >> time/MPIMedium40_6_$filterSize"
        eval "mpirun ./bin/mpiFilter data/large50 out $filterSize >> time/MPILarge40_6_$filterSize"
        eval "mpirun ./bin/mpiFilter data/smallsize out $filterSize >> time/MPISmallsize60_6_$filterSize"
        eval "mpirun ./bin/mpiFilter data/largesize out $filterSize >> time/MPILargesize60_6_$filterSize"
        #eval "mpirun ./bin/mpiFilter data/small10 out $filterSize >> time/serialSmall$filterSize"
        #eval "mpirun ./bin/mpiFilter data/medium30 out $filterSize >> time/serialMedium$filterSize"
        #eval "mpirun ./bin/mpiFilter data/large50 out $filterSize >> time/serialLarge$filterSize"
#        eval "./bin/ompFilter data out $filterSize >> time/omp$filterSize"
#        eval "mpiexec -n 4 ./bin/mpiFilter data out $filterSize >> time/mpi$filterSize"
#        eval "mpiexec -n 4 ./bin/hybridFilter data out $filterSize >> time/hybrid$filterSize"
    done
done