/**
 * @author Bradley Culligan
 * @author Michael Scott
 * @author Jared May
 * @version October 2022
 *
 * Receives a set of arguments and a number of arguments to check for valid argument passing
 */

// User-defined headers
#include "argumentChecker.h"

// Macros
#define NUM_ARGS 3 // exactly 3 args must be passed
#define QUIT_MSG "Incorrect arguments provided\nQuitting program\n"

/**
 * Checks if number of arguments provided is correct
 */
bool isValidNumberOfArgs(int argsNumProvided)
{
    bool flag = true;

    if (argsNumProvided != NUM_ARGS)
    {
        fprintf(stderr, "Invalid number of arguments: %d\n", argsNumProvided);
        flag = false;
    }

    return flag;
}

/**
 * Checks that dir is a valid directory
 */
bool isDir(char *dir)
{
    bool flag = true;
    DIR *directory;

    if ((directory = opendir(dir)) == NULL)
    {
        fprintf(stderr, "Invalid directory: can't open %s\n", dir);
        flag = false;
    }
    closedir(directory);

    return flag;
}

/**
 * @brief Checks if the filter width is a valid integer choice
 *
 * @param filterWidth Width of window to be checked
 * @return boolean depending on if the width is valid or not
 */
bool isFilterWidth(char *filterWidth)
{
    bool flag = true;

    int windowSize = atoi(filterWidth);
    if (windowSize > 25 || windowSize < 3 || windowSize % 2 == 0)
    {
        fprintf(stderr, "Invalid window size entered (%d) - must be odd in range [3, 25]\n", windowSize);
        flag = false;
    }

    return flag;
}

/**
 * Full check for valid arguments based on certain criteria
 */
bool isValidArguments(int argCount, char *args[])
{
    // Skip the executable name
    --argCount;
    ++args;

    char *inDir = args[0];
    char *outDir = args[1];
    char *filterWidth = args[2];

    return (isValidNumberOfArgs(argCount) && isDir(inDir) && isDir(outDir) && isFilterWidth(filterWidth));
}

/**
 * Ouput closing message and quit the program due to argument issues
 */
void quitProgram()
{
    fprintf(stderr, QUIT_MSG);
    exit(1);
}
