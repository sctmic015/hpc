# Makefile implementation for HPC Assignment Median Filter
# C bulk image processing with median filter
# @author Bradley Culligan
# @author Michael Scott
# @author Jared May
# @version October 2022

# Set up variables for directories
BIN=bin
SRC=src
INC=include
OBJ=build
OUT=out
# Basic flags
CC=gcc
MPICC=mpicc
INCLUDES=-I$(INC)
CCFLAGS=-fopenmp
LIBS=-lm

# List of files and paths used, stored as variables
P1=serialFilter
P2=ompFilter
P3=mpiFilter
P4=hybridFilter
TARGETS=$(BIN)/$(P1) $(BIN)/$(P2) $(BIN)/$(P3) $(BIN)/$(P4)
SRCFILES=$(wildcard $(SRC)/*.c)
OBJFILES=$(OBJ)/argumentChecker.o $(OBJ)/utilities.o
P1OBJS=$(OBJFILES) $(OBJ)/serialFilter.o
P2OBJS=$(OBJFILES) $(OBJ)/ompFilter.o
P3OBJS=$(OBJFILES) $(OBJ)/mpiFilter.o
P4OBJS=$(OBJFILES) $(OBJ)/hybridFilter.o
HEADFILES=$(wildcard $(INC)/*.h)

# Default build to make all executables
build: $(TARGETS)
	@echo Compilation complete! Please run ./executable with appropriate arguments to begin execution, where executable is one of $(TARGETS)...

# Serial
$(BIN)/$(P1): $(P1OBJS)
	$(CC) $^ -o $@ $(LIBS)

# OpenMP
$(BIN)/$(P2): $(P2OBJS)
	$(CC) $(CCFLAGS) $^ -o $@ $(LIBS)

# MPI
$(BIN)/$(P3): $(P3OBJS)
	$(MPICC) -openmpi-mp $^ -o $@ $(LIBS)

# Hybrid
$(BIN)/$(P4): $(P4OBJS)
	$(MPICC) $(CCFLAGS) $^ -o $@ $(LIBS)

# Source file checking and compiling
$(OBJ)/%.o: $(SRC)/%.c
	$(MPICC) -openmpi-mp $(CCFLAGS) $(INCLUDES) -c $< -o $@ $(LIBS)

.PHONY: clean rebuild cleanOut

# Remove all object files, executables, created outputs and hidden files
clean:
	@rm -rf $(OBJ)/*.o $(BIN)/* $(OUT)/* .DS_Store
	@echo Clean!

# Clean out the system and build it again
rebuild: clean build

# Clean all files stored in out (to clean up the workspace without having to clean binaries)
cleanOut:
	@rm -rf $(OUT)/*
	@echo "Output directory now clean!"

# Clean all files stored in time (to clean up the workspace without having to clean binaries)
cleanTime:
	@rm -rf time/*
	@echo "Time directory now clean!"