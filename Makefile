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
CC=clang
INCLUDES=-I$(INC)
CCFLAGS=

# List of files and paths used, stored as variables
EXE=medianFilter
TARGET=$(BIN)/$(EXE)
SRCFILES=$(wildcard $(SRC)/*.c)
OBJFILES=$(SRCFILES:$(SRC)/%.c=$(OBJ)/%.o)
HEADFILES=$(wildcard $(INC)/*.h)

# Default build to make executable
build: $(TARGET)
	@echo Compilation complete! Please run ./$(TARGET) with appropriate arguments to begin execution...

# Object file checking and linking
$(TARGET): $(OBJFILES)
	$(CC) $(CCFLAGS) $^ -o $@

# Source file checking and compiling
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

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