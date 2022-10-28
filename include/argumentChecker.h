/**
 * @author Bradley Culligan
 * @author Michael Scott
 * @author Jared May
 * @version October 2022
 * @version April 2021
 *
 * Header for argument checking
 * Check a set of arguments and a number of arguments for valid argument passing
 */

#ifndef _ARGUMENT_CHECKER
#define _ARGUMENT_CHECKER

// System headers
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Methods
bool isValidNumberOfArgs(int argsNumProvided);
bool isDir(char * dir);
bool isFilterWidth(char * filterWidth);
bool isValidArguments(int argCount, char *args[]);

void quitProgram();

#endif