import matplotlib.pyplot as plt
import seaborn as sb
import numpy as np
import pandas as pd
import os
from os import listdir
from os.path import isfile, join


# Create DataFrames for all Sets of data
def readData():
    filesSerial = [f for f in listdir(r"C:\Users\micha\CLionProjects\hpc\time\Serial") if isfile(join(r"C:\Users\micha\CLionProjects\hpc\time\Serial", f))]
    filesOMP = [f for f in listdir(r"C:\Users\micha\CLionProjects\hpc\time\OpenMP") if isfile(join(r"C:\Users\micha\CLionProjects\hpc\time\OpenMP", f))]
    # filesMPI = [f for f in listdir("out/mpi") if isfile(join("out/mpi", f))]
    # filesHybrid = [f for f in listdir("out/hybrid") if isfile(join("out/hybrid", f))]
    serialData = pd.DataFrame()
    serialMean = pd.DataFrame()
    ompData = pd.DataFrame()
    ompMean = pd.DataFrame()
    for i in filesSerial:
        serialData[i] = (np.loadtxt(rf"C:\Users\micha\CLionProjects\hpc\time\Serial\{i}"))
        serialMean[i] = np.array([np.mean(serialData[i])])
    for i in filesOMP:
        ompData[i] = (np.loadtxt(rf"C:\Users\micha\CLionProjects\hpc\time\OpenMP\{i}"))
        ompMean[i] = np.array([np.mean(ompData[i])])
    return serialData, ompData, serialMean, ompMean

def ompSpeedUpGraphs(serialMean, ompMean):
    dataTest = ['Small, Medium, Large, Smallsize, Largesize']
    # all speed-ups for window size 9
    small9 = np.array([serialMean['serialSmall9']/ompMean['ompSmall209'],
                      serialMean['serialSmall9']/ompMean['ompSmall409'],
                      serialMean['serialSmall9']/ompMean['ompSmall609']])
    medium9 = np.array([serialMean['serialMedium9']/ompMean['ompMedium209'],
                       serialMean['serialMedium9']/ompMean['ompMedium409'],
                       serialMean['serialMedium9']/ompMean['ompMedium609']])
    large9 = np.array([serialMean['serialLarge9']/ompMean['ompLarge209'],
                       serialMean['serialLarge9']/ompMean['ompLarge409'],
                       serialMean['serialLarge9']/ompMean['ompLarge609']])
    largeSize9 = np.array([serialMean['serialLargesize9']/ompMean['ompLargesize209'],
                       serialMean['serialLargesize9']/ompMean['ompLargesize409'],
                       serialMean['serialLargesize9']/ompMean['ompLargesize609']])
    smallSize9 = np.array([serialMean['serialSmallsize9']/ompMean['ompSmallsize209'],
                       serialMean['serialSmallsize9']/ompMean['ompSmallsize409'],
                       serialMean['serialSmallsize9']/ompMean['ompSmallsize609']])


    plt.plot(small9)
    plt.plot(medium9)
    plt.plot(large9)
    plt.plot(smallSize9)
    plt.plot(largeSize9)
    plt.title("OpenMP Speed Up for Window Size 9")
    plt.ylabel("Speedup")
    plt.xlabel("Core Count")
    plt.xticks(np.arange(3), ["20", "40", "60"])
    plt.legend(["Small", "Medium", "Large", "SmallSize", "LargeSize"])
    plt.show()

serialData, ompData, serialMean, ompMean = readData()
ompSpeedUpGraphs(serialMean, ompMean)



