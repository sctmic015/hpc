/**
 * @author Bradley Culligan
 * @author Michael Scott
 * @author Jared May
 * @version October 2022
 *
 * Header for utilities used in filtering
 * Provides commonly used functions such as sorts and merges etc
 */

// Headers
#include "utilities.h"

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r] */
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the sub-array of
arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

/* Function to sort an array using insertion sort */
void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1],
           that are greater than key,
           to one position ahead of
           their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/* Get median value in window */
int medianFliter(int window[], int n)
{
    mergeSort(window, 0, n - 1);
    // insertionSort(window, n);
    int median = window[(n + 1) / 2];
    return median;
}

// A utility function to print array of size n
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Function to determine if directory entry is a hidden file
bool hiddenFile(struct dirent *dp)
{
    return (dp->d_name[0] == '.');
}

// Function to determine if file is .png or .jpeg or .jpg
bool pngOrJpeg(char *file)
{
    char *dot = strrchr(file, '.');
    return (dot && (!strcmp(dot, ".png") || !strcmp(dot, ".jpeg") || !strcmp(dot, ".jpg")));
}

// Counts the number of files that should be run through the filter
int getNumberOfFiles(char *inDir)
{
    int count = 0;
    struct dirent *dp;
    DIR *dir = opendir(inDir);

    while ((dp = readdir(dir)) != NULL)
    {
        if (!hiddenFile(dp) && pngOrJpeg(dp->d_name))
        {
            count++;
        }
    }

    closedir(dir);

    return count;
}

// Gets the list of files to be filtered
void getListOfFiles(char *inDir, char *files[])
{
    // Populate files array
    struct dirent *dp;
    DIR *dir = opendir(inDir);

    int fileNum = 0;

    while ((dp = readdir(dir)) != NULL)
    {
        if (!hiddenFile(dp) && pngOrJpeg(dp->d_name))
        {
            files[fileNum] = (char *)malloc(strlen(dp->d_name) + 1);
            strcpy(files[fileNum], dp->d_name);
            fileNum++;
        }
    }

    closedir(dir);
}

// Create a combined file path name
char *createCombinedName(char filePath[], char fileName[])
{
    char *combined = malloc(strlen(filePath) + strlen(fileName));
    strcpy(combined, filePath);
    strcat(combined, "/");
    strcat(combined, fileName);

    return combined;
}