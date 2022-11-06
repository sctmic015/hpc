import matplotlib.pyplot as plt
import seaborn as sb
import numpy as np
import pandas as pd
from os import listdir, stat
from os.path import isfile, join

PATH = "./time/"

def readData():
    ROOT = "./time/"
    SERIAL_PATH = "./time/Serial/"
    OMP_PATH = "./time/OpenMP/"
    MPI_PATH = "./time/MPI/"
    
    
    serialRaw = pd.DataFrame()
    ompRaw = pd.DataFrame()
    mpiRaw = pd.DataFrame()
    
    serialMeans = pd.DataFrame()
    ompMeans = pd.DataFrame()
    mpiMeans = pd.DataFrame()
    
    ALGS = {"Serial":(SERIAL_PATH, serialRaw, serialMeans), "OpenMP":(OMP_PATH, ompRaw, ompMeans), "MPI":(MPI_PATH, mpiRaw, mpiMeans)}
    
    all_files = {}
    
    for alg in ALGS:
        print("Reading Files...", alg)
        path = ALGS[alg][0]
        all_files[alg]=([f for f in listdir(path) if
                  (isfile(join(path, f)) and stat(join(path, f)).st_size!=0)])
        print("Files Read ->", alg)

        print("Create Dataframes...", alg)
        for file in all_files[alg]:
            path = ALGS[alg][0]+file
            # path = PATH+file
            currFile = open(path, "r")
            
            # print("\nCurrFile:", path)
            ALGS[alg][1][file] = np.loadtxt(currFile)
            ALGS[alg][2][file] = np.array([np.mean(ALGS[alg][1][file])])
            currFile.close()
        print("Created Dataframes ->", alg, "\n")
        
    return serialRaw, ompRaw, mpiRaw, serialMeans, ompMeans, mpiMeans
        
        
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
                          serialMean['serialSmall' + data] / ompMean['ompSmall60' + data]], 'float64')
        medium = np.array([1, serialMean['serialMedium' + data] / ompMean['ompMedium20' + data],
                           serialMean['serialMedium' + data] / ompMean['ompMedium40' + data],
                           serialMean['serialMedium' + data] / ompMean['ompMedium60' + data]], 'float64')
        large = np.array([1, serialMean['serialLarge' + data] / ompMean['ompLarge20' + data],
                          serialMean['serialLarge' + data] / ompMean['ompLarge40' + data],
                          serialMean['serialLarge' + data] / ompMean['ompLarge60' + data]], 'float64')
        largeSize = np.array([1, serialMean['serialLargesize' + data] / ompMean['ompLargesize20' + data],
                              serialMean['serialLargesize' + data] / ompMean['ompLargesize40' + data],
                              serialMean['serialLargesize' + data] / ompMean['ompLargesize60' + data]], 'float64')
        smallSize = np.array([1, serialMean['serialSmallsize' + data] / ompMean['ompSmallsize20' + data],
                              serialMean['serialSmallsize' + data] / ompMean['ompSmallsize40' + data],
                              serialMean['serialSmallsize' + data] / ompMean['ompSmallsize60' + data]], 'float64')

        # print(small)
        # print(medium)
        # print(large)
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
    plt.savefig("./test/graphs/OpenMPResults.jpg")
    print(serialMean['serialLargesize15'] / ompMean['ompLargesize6015'])

def mpiSpeedUpGraphs(serialMean, mpiMean):
    plt.rcParams['figure.figsize'] = [18,5]
    plt.rcParams['figure.dpi'] = 300
    
    windows = ['3', '9', '15']
    nodes = ['1','2','4','6']
    cores = ['20', '40', '60']
    
    fig, ax = plt.subplots(nrows = 1, ncols=3)
    
    # MAKE A GRAPH PER NODE
    for node in nodes:
        print("\nPlotting node", node, "...")
        countCol = 0
        for data in windows:
            print("Plotting window size", data, "...")
            small = np.array([1])
            medium = np.array([1])
            large = np.array([1])
            largeSize = np.array([1])
            smallSize = np.array([1])
            for core in cores:
                if 'MPISmall'+ core + '_' + node + "_" + data in mpiMean:
                    small = np.append(small, [serialMean['serialSmall' + data] / mpiMean['MPISmall'+ core + '_' + node + "_" + data]])
                else:
                    small = np.append(small, [0])
                if 'MPIMedium'+ core + '_' + node + "_" + data in mpiMean:
                    medium = np.append(medium, [serialMean['serialMedium' + data] / mpiMean['MPIMedium'+ core + '_' + node + "_" + data]])
                else:
                    medium = np.append(medium, [0])
                if 'MPILarge'+ core + '_' + node + "_" + data in mpiMean:
                    large = np.append(large, [serialMean['serialLarge' + data] / mpiMean['MPILarge'+ core + '_' + node + "_" + data]])
                else:
                    large = np.append(large, [0])
                if 'MPILargesize'+ core + '_' + node + "_" + data in mpiMean:
                    largeSize = np.append(largeSize, [serialMean['serialLargesize' + data] / mpiMean['MPILargesize'+ core + '_' + node + "_" + data]])
                else:
                    largeSize = np.append(largeSize, [0])
                if 'MPISmallsize'+ core + '_' + node + "_" + data in mpiMean:
                    smallSize = np.append(smallSize, [serialMean['serialSmallsize' + data] / mpiMean['MPISmallsize'+ core + '_'  + node + "_" + data]])
                else:
                    smallSize = np.append(smallSize, [0])    
            print("Small", len(small))
            print("med", len(medium))
            print("large", len(large))
            # print("small")
            # print(small)
            # print("medium")
            # print(medium)
            # print("large")
            # print(large)
            
            
        
            
            ax[countCol].plot(small)
            ax[countCol].plot(medium)
            ax[countCol].plot(large)
            ax[countCol].plot(smallSize)
            ax[countCol].plot(largeSize)
            
            ax[countCol].set_title(("MPI Speed Up for Window Size " + data +" and nodes " + node))
            ax[countCol].set_ylabel("Speedup")
            ax[countCol].set_xlabel("Core Count")
            ax[countCol].set_xticks(np.arange(4), ["1", "20", "40", "60"])
            ax[countCol].legend(["Small", "Medium", "Large", "SmallSize", "LargeSize"])
            
            countCol += 1
        plt.savefig("./test/graphs/MPIResults"+node+".jpg")

                        
if __name__ == "__main__":
    serialData, ompData, mpiData, serialMean, ompMean, mpiMean = readData()
    ompSpeedUpGraphs(serialMean, ompMean)
    mpiSpeedUpGraphs(serialMean, mpiMean)
    

# def get_files():
#     files = os.listdir(PATH)

#     print("Files in path are", files)

#     # alg_type = ["Sequential", "OMP", "MPI"]
#     # n_imgs = {"small":"10", "medium":"30", "large":"50"}
#     # filter_size = ["3", "9", "15", "25"]
#     # n_cores = ["10", "20", "40"]
#     # n_nodes = ["2", "4"]
#     file_info = []

#     for file in files:
#         print("\nFile:", file)
#         parse_filename(filename = file)

# def parse_filename(filename = ""):
#     if filename == "":
#         print("No file given!")
#         exit(1)
#     else:
#         alg_type = ["serial", "omp", "mpi", "hybrid"]
#         n_imgs = {"Small":"10", "Medium":"30", "Large":"50"}
#         filter_size = ["3", "9", "15", "25"]
#         n_cores = ["10", "20", "40"]
#         n_nodes = ["2", "4"]
#         index_end = len(filename)
#         # test_info = {"alg":"", "n_img":"", "filter":"", "n_cores":"", "n_nodes":""}
#         test_info = {}
        
#         for alg in alg_type:
#             if (filename.find(alg) != -1):
#                 is_alg = alg
#                 test_info["alg"]=is_alg
#                 for img in n_imgs:
#                     #print(img)
#                     if (filename.find(img) != -1):
#                         is_img = img
#                         test_info["n_img"]=n_imgs[is_img]
#                         for filter in filter_size:
#                             index_filter = filename.find(filter)
#                             if (index_filter != -1):
#                                 index_start = index_filter+1
#                                 is_filter = filter
#                                 test_info["filter"]=is_alg
            
#                                 print("Alg is:", is_alg)
#                                 print("Image is:", is_img, "Value is:", n_imgs[is_img])
#                                 print("Filter is:", is_filter)    
                                
#                                 if is_alg == "omp":
#                                     for core in n_cores:
#                                         if (filename.find(core,index_start, index_end) != -1):
#                                             is_core = core
#                                             test_info["n_cores"]=is_core
#                                             print("Core is:", is_core)
#                                 elif is_alg == "mpi":
#                                     for node in n_nodes:
#                                         if (filename.find(node,index_start, index_end) != -1):
#                                             is_node = node
#                                             test_info["n_nodes"]=is_node
#                                             print("Node is:", is_node)
#                                 elif is_alg == "hybrid":
#                                     for core in n_cores:
#                                         if (filename.find(core,index_start, index_end) != -1):
#                                             is_core = core
#                                             test_info["n_cores"]=is_core
#                                             print("Core is:", is_core)
#                                     for node in n_nodes:
#                                         if (filename.find(node,index_start, index_end) != -1):
#                                             is_node = node
#                                             test_info["n_nodes"]=is_node
#                                             print("Node is:", is_node)
                                            
#         print(test_info)
#         return test_info        
        

    
# \item[Input Size (No. of Images):] Small (10), Medium (30), and Large (50) images.
    # \item[Filter Size:] 3, 9, 15, and 25.
    # \item[Cores:] 10, 20, and 40 cores.
    # \item[Nodes:] 2 and 4 nodes.
    
    