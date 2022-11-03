#! /bin/bash
# Used to call each median filter program 10 times for statistical validity when varying filter sizes.
# Testing with different data input file sizes should be done with separate calls to this script.

filterSizes=(3 9 15 25)

for filterSize in "${filterSizes[@]}"
do
    for iteration in {1..3}
    do
        eval "./bin/serialFilter data/small10 out $filterSize >> time/serialSmall$filterSize"
        eval "./bin/serialFilter data/medium30 out $filterSize >> time/serialMedium$filterSize"
        eval "./bin/serialFilter data/large50 out $filterSize >> time/serialLarge$filterSize"
#        eval "./bin/ompFilter data out $filterSize >> time/omp$filterSize"
#        eval "mpiexec -n 4 ./bin/mpiFilter data out $filterSize >> time/mpi$filterSize"
#        eval "mpiexec -n 4 ./bin/hybridFilter data out $filterSize >> time/hybrid$filterSize"
    done
done