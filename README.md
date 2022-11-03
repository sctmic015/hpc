# HPC Median Filter Assignment
### Michael Scott, Bradley Culligan, Jared May

This is a series of programs setup to run a median filter of user-defined size on a directory of images. 
The intention is to measure speedup with regards to a serial, OpenMP, MPI, and hybrid OpenMP and MPI solution 
for running the filter of a directory of images. Further details of the solutions involved and the analysis of 
the results achieved can be found in the report. Below is a brief introduction to running the program.

## Launching the Programs

A Makefile is supplied for convenience of compiling the programs. Running `make` from the root directory will 
generate all executables. Alternatively, each executable can be specified to be compiled by running `make bin/{exe}` 
where {exe} can be one of the following:
* serialFilter
* ompFilter
* mpiFilter
* hybridFilter

It is recommened to simply run `make`. Each file can then be run individually by launching the program with `./bin/{exe}`, 
where {exe} is one of the stipulated from above, followed by three arguments (in order):

{inputFolderName} {outputFolderName} {filterWidth}

where 

{inputFolderName} is the name of the directory containing the input images (only) in .png and/or .jpeg format;

{outputFolderName} is the name of the directory where the smoothed images should be placed;

{filterWidth} is the width of the square filter window - this must be an odd, positive integer >= 3.

Note that recommened directories have been supplied - `data` for input images and `out` for output images so it is recommended 
to put the images you would like filtered into the `data` directory and then supply the `data` and `out` arguments to the program. 
An example execution might look like:  
`./bin/serialFilter data out 5` 

Further, a bash script is supplied for automated running of each program in order to provide the necessary statistical confidence 
in a varied set of parameter results such that speedup graphs over the changed parameters could be plotted and analysed.