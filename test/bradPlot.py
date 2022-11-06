import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from os import listdir
from os.path import isfile, join


# Create DataFrames for all MPI data
def readData():
    serialPath = "../time/Serial"
    mpiPath = "../time/important_mpi_results"

    filesSerial = [f for f in listdir(rf"{serialPath}") if
                   isfile(join(serialPath, f))]
    filesMPI = [f for f in listdir(rf"{mpiPath}") if
                isfile(join(mpiPath, f))]
    serialData = pd.DataFrame()
    serialMean = pd.DataFrame()
    mpiData = pd.DataFrame()
    mpiMean = pd.DataFrame()
    for i in filesSerial:
        serialData[i] = (np.loadtxt(rf"{serialPath}/{i}"))
        serialMean[i] = np.array([np.mean(serialData[i])])
    for i in filesMPI:
        mpiData[i] = (np.loadtxt(rf"{mpiPath}/{i}"))
        mpiMean[i] = np.array([np.mean(mpiData[i])])
    return serialData, serialMean, mpiData, mpiMean


def ompStyleSpeedUpGraphsForMPI(serialMean, mpiMean):
    plt.rcParams['figure.figsize'] = [18, 8]
    plt.rcParams['figure.dpi'] = 300
    dataTest = ['3', '9']
    # all speed-ups for window size 9
    fig, ax = plt.subplots(ncols=2)
    count = 0
    for data in dataTest:
        small = np.array([1, serialMean['serialSmall' + data] / mpiMean['MPISmall20_1_' + data],
                          serialMean['serialSmall' + data] /
                          mpiMean['MPISmall40_1_' + data],
                          serialMean['serialSmall' + data] / mpiMean['MPISmall60_1_' + data]])
        medium = np.array([1, serialMean['serialMedium' + data] / mpiMean['MPIMedium20_1_' + data],
                           serialMean['serialMedium' + data] /
                           mpiMean['MPIMedium40_1_' + data],
                           serialMean['serialMedium' + data] / mpiMean['MPIMedium60_1_' + data]])
        large = np.array([1, serialMean['serialLarge' + data] / mpiMean['MPILarge20_1_' + data],
                          serialMean['serialLarge' + data] /
                          mpiMean['MPILarge40_1_' + data],
                          serialMean['serialLarge' + data] / mpiMean['MPILarge60_1_' + data]])
        largeSize = np.array([1, serialMean['serialLargesize' + data] / mpiMean['MPILargesize20_1_' + data],
                              serialMean['serialLargesize' + data] /
                              mpiMean['MPILargesize40_1_' + data],
                              serialMean['serialLargesize' + data] / mpiMean['MPILargesize60_1_' + data]])
        smallSize = np.array([1, serialMean['serialSmallsize' + data] / mpiMean['MPISmallsize20_1_' + data],
                              serialMean['serialSmallsize' + data] /
                              mpiMean['MPISmallsize40_1_' + data],
                              serialMean['serialSmallsize' + data] / mpiMean['MPISmallsize60_1_' + data]])

        ax[count].plot(small)
        ax[count].plot(medium)
        ax[count].plot(large)
        ax[count].plot(smallSize)
        ax[count].plot(largeSize)
        ax[count].set_title("MPI Speed Up for Window Size " +
                            data + " as Cores Increase on One Node")
        ax[count].set_ylabel("Speedup")
        ax[count].set_xlabel("Core Count")
        ax[count].set_xticks(np.arange(4), ["1", "20", "40", "60"])
        ax[count].legend(
            ["Small", "Medium", "Large", "SmallSize", "LargeSize"])
        count += 1
    # plt.show()
    plt.savefig("graphs/MPIOneNodeCoreChange.jpg")


def mpiSpeedUpGraphsNodeChanges(serialMean, mpiMean):
    plt.rcParams['figure.figsize'] = [20, 5]
    plt.rcParams['figure.dpi'] = 300
    dataTest = ['3', '9', '15']
    fig, ax = plt.subplots(ncols=3)
    count = 0
    for data in dataTest:
        small = np.array([1, serialMean['serialSmall' + data] / mpiMean['MPISmall60_1_' + data],
                          serialMean['serialSmall' + data] /
                          mpiMean['MPISmall60_2_' + data],
                          serialMean['serialSmall' + data] /
                          mpiMean['MPISmall60_4_' + data],
                          serialMean['serialSmall' + data] / mpiMean['MPISmall60_6_' + data]])
        medium = np.array([1, serialMean['serialMedium' + data] / mpiMean['MPIMedium60_1_' + data],
                           serialMean['serialMedium' + data] /
                           mpiMean['MPIMedium60_2_' + data],
                           serialMean['serialMedium' + data] /
                           mpiMean['MPIMedium60_4_' + data],
                           serialMean['serialMedium' + data] / mpiMean['MPIMedium60_6_' + data]])
        large = np.array([1, serialMean['serialLarge' + data] / mpiMean['MPILarge60_1_' + data],
                          serialMean['serialLarge' + data] /
                          mpiMean['MPILarge60_2_' + data],
                          serialMean['serialLarge' + data] /
                          mpiMean['MPILarge60_4_' + data],
                          serialMean['serialLarge' + data] / mpiMean['MPILarge60_6_' + data]])
        largeSize = np.array([1, serialMean['serialLargesize' + data] / mpiMean['MPILargesize60_1_' + data],
                              serialMean['serialLargesize' + data] /
                              mpiMean['MPILargesize60_2_' + data],
                              serialMean['serialLargesize' + data] /
                              mpiMean['MPILargesize60_4_' + data],
                              serialMean['serialLargesize' + data] / mpiMean['MPILargesize60_6_' + data]])
        smallSize = np.array([1, serialMean['serialSmallsize' + data] / mpiMean['MPISmallsize60_1_' + data],
                              serialMean['serialSmallsize' + data] /
                              mpiMean['MPISmallsize60_2_' + data],
                              serialMean['serialSmallsize' + data] /
                              mpiMean['MPISmallsize60_4_' + data],
                              serialMean['serialSmallsize' + data] / mpiMean['MPISmallsize60_6_' + data]])

        ax[count].plot(small)
        ax[count].plot(medium)
        ax[count].plot(large)
        ax[count].plot(smallSize)
        ax[count].plot(largeSize)
        ax[count].set_title("MPI Speed Up for Window Size " +
                            data + " as Node Count Changes")
        ax[count].set_ylabel("Speedup")
        ax[count].set_xlabel("Node Count (Always 60 Cores)")
        ax[count].set_xticks(np.arange(5), ["Serial", "1", "2", "4", "6"])
        ax[count].legend(
            ["Small", "Medium", "Large", "SmallSize", "LargeSize"])
        count += 1
    # plt.show()
    plt.savefig("graphs/MPINodeChangeOn60Cores.jpg")


def mpiSpeedUpGraphs(serialMean, mpiMean):
    plt.rcParams['figure.figsize'] = [18, 15]
    plt.rcParams['figure.dpi'] = 300
    windows = ['3', '9', '15']
    nodes = ['4']
    # all speed-ups for window size 9
    fig, ax = plt.subplots(nrows=1, ncols=3)
    countCol = 0
    countRow = 0
    for node in nodes:
        countCol = 0
        for data in windows:
            small = np.array([1, serialMean['serialSmall' + data] / mpiMean['mpiSmall20_' + nodes + "_" + data],
                              serialMean['serialSmall' + data] /
                              mpiMean['mpiSmall40_' + nodes + "_" + data],
                              serialMean['serialSmall' + data] / mpiMean['mpiSmall60_' + nodes + "_" + data]])
            medium = np.array([1, serialMean['serialMedium' + data] / mpiMean['mpiMedium20_' + nodes + "_" + data],
                               serialMean['serialMedium' + data] /
                               mpiMean['mpiMedium40_' + nodes + "_" + data],
                               serialMean['serialMedium' + data] / mpiMean['mpiMedium60_' + nodes + "_" + data]])
            large = np.array([1, serialMean['serialLarge' + data] / mpiMean['mpiLarge20_' + nodes + "_" + data],
                              serialMean['serialLarge' + data] /
                              mpiMean['mpiLarge40_' + nodes + "_" + data],
                              serialMean['serialLarge' + data] / mpiMean['mpiLarge60_' + nodes + "_" + data]])
            largeSize = np.array([1, serialMean['serialLargesize' + data] / mpiMean['mpiLargesize20_' + nodes + "_" + data],
                                  serialMean['serialLargesize' + data] /
                                  mpiMean['mpiLargesize40_' +
                                          nodes + "_" + data],
                                  serialMean['serialLargesize' + data] / mpiMean['mpiLargesize60_' + nodes + "_" + data]])
            smallSize = np.array([1, serialMean['serialSmallsize' + data] / mpiMean['mpiSmallsize20_' + nodes + "_" + data],
                                  serialMean['serialSmallsize' + data] /
                                  mpiMean['mpiSmallsize40_' +
                                          nodes + "_" + data],
                                  serialMean['serialSmallsize' + data] / mpiMean['mpiSmallsize60_' + nodes + "_" + data]])

            ax[countRow, countCol].plot(small)
            ax[countRow, countCol].plot(medium)
            ax[countRow, countCol].plot(large)
            ax[countRow, countCol].plot(smallSize)
            ax[countRow, countCol].plot(largeSize)
            ax[countRow, countCol].set_title(
                "MPI Speed Up for Window Size " + data + " and modes " + node)
            ax[countRow, countCol].set_ylabel("Speedup")
            ax[countRow, countCol].set_xlabel("Core Count")
            ax[countRow, countCol].set_xticks(
                np.arange(4), ["1", "20", "40", "60"])
            ax[countRow, countCol].legend(
                ["Small", "Medium", "Large", "SmallSize", "LargeSize"])
            countCol += 1
        countCol += 1
    # plt.show()
    plt.savefig("graphs/MPIResults.jpg")
   # print(serialMean['serialLargesize15'] / ompMean['ompLargesize6015'])


serialData, serialMean, mpiData, mpiMean = readData()
ompStyleSpeedUpGraphsForMPI(serialMean, mpiMean)
mpiSpeedUpGraphsNodeChanges(serialMean, mpiMean)
#ompSpeedUpGraphs(serialMean, ompMean)
