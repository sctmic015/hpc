import matplotlib.pyplot as plt
import seaborn as sb
import numpy as np
import pandas as pd
import os

PATH = "./time"

def get_files():
    files = os.listdir(PATH)

    print("Files in path are", files)

    # alg_type = ["Sequential", "OMP", "MPI"]
    # n_imgs = {"small":"10", "medium":"30", "large":"50"}
    # filter_size = ["3", "9", "15", "25"]
    # n_cores = ["10", "20", "40"]
    # n_nodes = ["2", "4"]
    file_info = []

    for file in files:
        print("\nFile:", file)
        parse_filename(filename = file)

def parse_filename(filename = ""):
    if filename == "":
        print("No file given!")
        exit(1)
    else:
        alg_type = ["serial", "omp", "mpi", "hybrid"]
        n_imgs = {"Small":"10", "Medium":"30", "Large":"50"}
        filter_size = ["3", "9", "15", "25"]
        n_cores = ["10", "20", "40"]
        n_nodes = ["2", "4"]
        index_end = len(filename)
        # test_info = {"alg":"", "n_img":"", "filter":"", "n_cores":"", "n_nodes":""}
        test_info = {}
        
        for alg in alg_type:
            if (filename.find(alg) != -1):
                is_alg = alg
                test_info["alg"]=is_alg
                for img in n_imgs:
                    #print(img)
                    if (filename.find(img) != -1):
                        is_img = img
                        test_info["n_img"]=n_imgs[is_img]
                        for filter in filter_size:
                            index_filter = filename.find(filter)
                            if (index_filter != -1):
                                index_start = index_filter+1
                                is_filter = filter
                                test_info["filter"]=is_alg
            
                                print("Alg is:", is_alg)
                                print("Image is:", is_img, "Value is:", n_imgs[is_img])
                                print("Filter is:", is_filter)    
                                
                                if is_alg == "omp":
                                    for core in n_cores:
                                        if (filename.find(core,index_start, index_end) != -1):
                                            is_core = core
                                            test_info["n_cores"]=is_core
                                            print("Core is:", is_core)
                                elif is_alg == "mpi":
                                    for node in n_nodes:
                                        if (filename.find(node,index_start, index_end) != -1):
                                            is_node = node
                                            test_info["n_nodes"]=is_node
                                            print("Node is:", is_node)
                                elif is_alg == "hybrid":
                                    for core in n_cores:
                                        if (filename.find(core,index_start, index_end) != -1):
                                            is_core = core
                                            test_info["n_cores"]=is_core
                                            print("Core is:", is_core)
                                    for node in n_nodes:
                                        if (filename.find(node,index_start, index_end) != -1):
                                            is_node = node
                                            test_info["n_nodes"]=is_node
                                            print("Node is:", is_node)
                                            
        print(test_info)
        return test_info        
        
        
        
                        
if __name__ == "__main__":
    get_files()
    
# \item[Input Size (No. of Images):] Small (10), Medium (30), and Large (50) images.
    # \item[Filter Size:] 3, 9, 15, and 25.
    # \item[Cores:] 10, 20, and 40 cores.
    # \item[Nodes:] 2 and 4 nodes.
    
    