from os import listdir
from os.path import isfile, join
filesSerial = [f for f in listdir("dataOutSerial") if isfile(join("dataOutSerial", f))]
filesOMP = [f for f in listdir("dataOutOMP") if isfile(join("dataOutOMP", f))]
filesMPI = [f for f in listdir("dataOutMPI") if isfile(join("dataOutMPI", f))]
filesHybrid = [f for f in listdir("dataOutHybrid") if isfile(join("dataOutHybrid", f))]

OMPCount = 0
MPICount = 0
HybridCount = 0

for i in range(len(filesSerial)):
    if (filesSerial[i] != filesMPI[i] or filesSerial[i] != filesOMP[i] or filesSerial[i] != filesHybrid[i]):
        print("Differing Image sets")
        exit(0)
    if open("dataOutSerial/" + filesSerial[i], "rb").read() != open("dataOutOMP/" + filesOMP[i], "rb").read():
        OMPCount += 1
    if open("dataOutSerial/" + filesSerial[i], "rb").read() != open("dataOutMPI/" + filesMPI[i], "rb").read():
        MPICount += 1
    if open("dataOutSerial/" + filesSerial[i], "rb").read() != open("dataOutHybrid/" + filesHybrid[i], "rb").read():
        HybridCount += 1

print("Number of differing OMP Images: " + str(OMPCount))
print("Number of differing MPI Images: " + str(MPICount))
print("Number of differing Hybrid Images: " + str(HybridCount))

