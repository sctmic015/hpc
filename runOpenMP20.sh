#!/bin/sh
#SBATCH --partition=curie
#SBATCH --time=12:00:00
#SBATCH --nodes=1 --ntasks=20
#SBATCH --job-name="openMP 20 core"
#SBATCH --mail-user=sctmic015@myuct.ac.za
#SBATCH --mail-type=ALL

module load mpi/openmpi-4.0.1

filterSizes=(3 9 15)

for filterSize in "${filterSizes[@]}"
do
    for iteration in {1..2}
    do
#        eval "./bin/serialFilter data/small10 out $filterSize >> time/serialSmall$filterSize"
#        eval "./bin/serialFilter data/medium30 out $filterSize >> time/serialMedium$filterSize"
#        eval "./bin/serialFilter data/large50 out $filterSize >> time/serialLarge$filterSize"
        eval "./bin/ompFilter data/small10 out $filterSize >> time/ompSmall20$filterSize"
        eval "./bin/ompFilter data/medium30 out $filterSize >> time/ompMedium20$filterSize"
        eval "./bin/ompFilter data/large50 out $filterSize >> time/ompLarge20$filterSize"
        eval "./bin/ompFilter data/smallsize out $filterSize >> time/ompSmallsize20$filterSize"
        eval "./bin/ompFilter data/largesize out $filterSize >> time/ompLargesize20$filterSize"
#        eval "mpiexec -n 4 ./bin/mpiFilter data out $filterSize >> time/mpi$filterSize"
#        eval "mpiexec -n 4 ./bin/hybridFilter data out $filterSize >> time/hybrid$filterSize"
    done
done