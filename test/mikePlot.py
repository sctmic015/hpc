import matplotlib
import matplotlib.pyplot as plt
#plt.style.use('science')
import seaborn as sb
import numpy as np
import pandas as pd
import os
from os import listdir
from os.path import isfile, join


# Create DataFrames for all Sets of data
def readData():
    filesSerial = [f for f in listdir(r"C:\Users\micha\CLionProjects\hpc\time\Serial") if
                   isfile(join(r"C:\Users\micha\CLionProjects\hpc\time\Serial", f))]
    filesOMP = [f for f in listdir(r"C:\Users\micha\CLionProjects\hpc\time\OpenMP") if
                isfile(join(r"C:\Users\micha\CLionProjects\hpc\time\OpenMP", f))]
    filesMPI = [f for f in listdir(r"C:\Users\micha\CLionProjects\hpc\time\MPI") if
                isfile(join(r"C:\Users\micha\CLionProjects\hpc\time\MPI", f))]
    # filesMPI = [f for f in listdir("out/mpi") if isfile(join("out/mpi", f))]
    # filesHybrid = [f for f in listdir("out/hybrid") if isfile(join("out/hybrid", f))]
    serialData = pd.DataFrame()
    serialMean = pd.DataFrame()
    ompData = pd.DataFrame()
    ompMean = pd.DataFrame()
    mpiData = pd.DataFrame()
    mpiMean = pd.DataFrame()
    for i in filesSerial:
        serialData[i] = (np.loadtxt(rf"C:\Users\micha\CLionProjects\hpc\time\Serial\{i}"))
        serialMean[i] = np.array([np.mean(serialData[i])])
    for i in filesOMP:
        ompData[i] = (np.loadtxt(rf"C:\Users\micha\CLionProjects\hpc\time\OpenMP\{i}"))
        ompMean[i] = np.array([np.mean(ompData[i])])
    for i in filesMPI:
        mpiData[i] = (np.loadtxt(rf"C:\Users\micha\CLionProjects\hpc\time\MPI\{i}"))
        mpiMean[i] = np.array([np.mean(mpiData[i])])
    return serialData, ompData, serialMean, ompMean, mpiData, mpiMean


def ompSpeedUpGraphs(serialMean, ompMean):
    plt.rcParams['figure.figsize'] = [18,5]
    plt.rcParams['figure.dpi'] = 300
    dataTest = ['3', '9', '15']
    # all speed-ups for window size 9
    fig, ax = plt.subplots(ncols=3)
    count = 0
    for data in dataTest:
        small = np.array([1, serialMean['serialSmall' + data] / ompMean['ompSmall20' + data],
                          serialMean['serialSmall' + data] / ompMean['ompSmall40' + data],
                          serialMean['serialSmall' + data] / ompMean['ompSmall60' + data]])
        medium = np.array([1, serialMean['serialMedium' + data] / ompMean['ompMedium20' + data],
                           serialMean['serialMedium' + data] / ompMean['ompMedium40' + data],
                           serialMean['serialMedium' + data] / ompMean['ompMedium60' + data]])
        large = np.array([1, serialMean['serialLarge' + data] / ompMean['ompLarge20' + data],
                          serialMean['serialLarge' + data] / ompMean['ompLarge40' + data],
                          serialMean['serialLarge' + data] / ompMean['ompLarge60' + data]])
        largeSize = np.array([1, serialMean['serialLargesize' + data] / ompMean['ompLargesize20' + data],
                              serialMean['serialLargesize' + data] / ompMean['ompLargesize40' + data],
                              serialMean['serialLargesize' + data] / ompMean['ompLargesize60' + data]])
        smallSize = np.array([1, serialMean['serialSmallsize' + data] / ompMean['ompSmallsize20' + data],
                              serialMean['serialSmallsize' + data] / ompMean['ompSmallsize40' + data],
                              serialMean['serialSmallsize' + data] / ompMean['ompSmallsize60' + data]])

        ax[count].plot(small)
        ax[count].plot(medium)
        ax[count].plot(large)
        ax[count].plot(smallSize)
        ax[count].plot(largeSize)
        ax[count].set_title("OpenMP Speed Up for Window Size " + data)
        ax[count].set_ylabel("Speedup")
        ax[count].set_xlabel("Core Count")
        ax[count].set_xticks(np.arange(4), ["1", "20", "40", "60"])
        ax[count].legend(["Small", "Medium", "Large", "SmallSize", "LargeSize"])
        count += 1
    #plt.show()
    plt.savefig("graphs/OpenMPResults.jpg")
    print(serialMean['serialLargesize15'] / ompMean['ompLargesize6015'])

def mpiSpeedUpGraphs(serialMean, mpiMean):
    plt.rcParams['figure.figsize'] = [18,15]
    plt.rcParams['figure.dpi'] = 300
    windows = ['3', '9', '15']
    nodes = ['4']
    # all speed-ups for window size 9
    fig, ax = plt.subplots(nrows = 1, ncols=3)
    countCol = 0
    countRow = 0
    for node in nodes:
        countCol = 0
        for data in windows:
            small = np.array([1, serialMean['serialSmall' + data] / mpiMean['mpiSmall20_' + nodes + "_" + data],
                              serialMean['serialSmall' + data] / mpiMean['mpiSmall40_' + nodes + "_" + data],
                              serialMean['serialSmall' + data] / mpiMean['mpiSmall60_' + nodes + "_" + data]])
            medium = np.array([1, serialMean['serialMedium' + data] / mpiMean['mpiMedium20_' + nodes + "_" + data],
                               serialMean['serialMedium' + data] / mpiMean['mpiMedium40_' + nodes + "_" + data],
                               serialMean['serialMedium' + data] / mpiMean['mpiMedium60_' + nodes + "_" + data]])
            large = np.array([1, serialMean['serialLarge' + data] / mpiMean['mpiLarge20_' + nodes + "_" + data],
                              serialMean['serialLarge' + data] / mpiMean['mpiLarge40_' + nodes + "_" + data],
                              serialMean['serialLarge' + data] / mpiMean['mpiLarge60_' + nodes + "_" + data]])
            largeSize = np.array([1, serialMean['serialLargesize' + data] / mpiMean['mpiLargesize20_' + nodes + "_" + data],
                                  serialMean['serialLargesize' + data] / mpiMean['mpiLargesize40_' + nodes + "_" + data],
                                  serialMean['serialLargesize' + data] / mpiMean['mpiLargesize60_' + nodes + "_" + data]])
            smallSize = np.array([1, serialMean['serialSmallsize' + data] / mpiMean['mpiSmallsize20_' + nodes + "_" + data],
                                  serialMean['serialSmallsize' + data] / mpiMean['mpiSmallsize40_' + nodes + "_" + data],
                                  serialMean['serialSmallsize' + data] / mpiMean['mpiSmallsize60_' + nodes + "_" + data]])

            ax[countRow,countCol].plot(small)
            ax[countRow,countCol].plot(medium)
            ax[countRow,countCol].plot(large)
            ax[countRow,countCol].plot(smallSize)
            ax[countRow,countCol].plot(largeSize)
            ax[countRow,countCol].set_title("MPI Speed Up for Window Size " + data " and modes " + node)
            ax[countRow,countCol].set_ylabel("Speedup")
            ax[countRow,countCol].set_xlabel("Core Count")
            ax[countRow,countCol].set_xticks(np.arange(4), ["1", "20", "40", "60"])
            ax[countRow,countCol].legend(["Small", "Medium", "Large", "SmallSize", "LargeSize"])
            countCol += 1
        countCol += 1
    #plt.show()
    plt.savefig("graphs/MPIResults.jpg")
   # print(serialMean['serialLargesize15'] / ompMean['ompLargesize6015'])

serialData, ompData, serialMean, ompMean, mpiData, mpiMean = readData()
ompSpeedUpGraphs(serialMean, ompMean)
