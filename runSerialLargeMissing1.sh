#!/bin/sh
#SBATCH --partition=curie
#SBATCH --time=12:00:00
#SBATCH --nodes=1 --ntasks=2
#SBATCH --job-name="serial"
#SBATCH --mail-user=myxjar002@myuct.ac.za
#SBATCH --mail-type=ALL

module load mpi/openmpi-4.0.1

filterSizes=(15)

for filterSize in "${filterSizes[@]}"
do
    for iteration in {1..2}
    do
        #eval "./bin/serialFilter data/small10 out $filterSize >> time/serialSmall$filterSize"
        #eval "./bin/serialFilter data/medium30 out $filterSize >> time/serialMedium$filterSize"
         eval "./bin/serialFilter data/large50 out $filterSize >> time/serialLarge$filterSize"
        #eval "./bin/serialFilter data/smallsize out $filterSize >> time/serialSmallsize$filterSize"
        #eval "./bin/serialFilter data/largesize out $filterSize >> time/serialLargesize$filterSize"
        #eval "mpirun ./bin/mpiFilter data/small10 out $filterSize >> time/serialSmall$filterSize"
        #eval "mpirun ./bin/mpiFilter data/medium30 out $filterSize >> time/serialMedium$filterSize"
        #eval "mpirun ./bin/mpiFilter data/large50 out $filterSize >> time/serialLarge$filterSize"
#        eval "./bin/ompFilter data out $filterSize >> time/omp$filterSize"
#        eval "mpiexec -n 4 ./bin/mpiFilter data out $filterSize >> time/mpi$filterSize"
#        eval "mpiexec -n 4 ./bin/hybridFilter data out $filterSize >> time/hybrid$filterSize"
    done
done
