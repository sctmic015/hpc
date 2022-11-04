from os import listdir
from os.path import isfile, join
filesSerial = [f for f in listdir("out/serial") if isfile(join("out/serial", f))]
filesOMP = [f for f in listdir("out/omp") if isfile(join("out/omp", f))]
filesMPI = [f for f in listdir("out/mpi") if isfile(join("out/mpi", f))]
filesHybrid = [f for f in listdir("out/hybrid") if isfile(join("out/hybrid", f))]

OMPCount = 0
MPICount = 0
HybridCount = 0

for i in range(len(filesSerial)):
    if (filesSerial[i] != filesMPI[i] or filesSerial[i] != filesOMP[i] or filesSerial[i] != filesHybrid[i]):
        print("Differing Image sets")
        exit(0)
    if open("out/serial/" + filesSerial[i], "rb").read() != open("out/omp/" + filesOMP[i], "rb").read():
        OMPCount += 1
    if open("out/serial/" + filesSerial[i], "rb").read() != open("out/mpi/" + filesMPI[i], "rb").read():
        MPICount += 1
    if open("out/serial/" + filesSerial[i], "rb").read() != open("out/hybrid/" + filesHybrid[i], "rb").read():
        HybridCount += 1

print("Number of differing OMP Images: " + str(OMPCount))
print("Number of differing MPI Images: " + str(MPICount))
print("Number of differing Hybrid Images: " + str(HybridCount))

