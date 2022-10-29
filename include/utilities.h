/**
 * @author Bradley Culligan
 * @author Michael Scott
 * @author Jared May
 * @version October 2022
 *
 * Header for utilities used in filtering
 * Provides commonly used functions such as sorts and merges etc
 */

#ifndef _UTILITIES
#define _UTILITIES

// System headers
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

// Methods
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r);

/* l is for left index and r is right index of the sub-array of
arr to be sorted */
void mergeSort(int arr[], int l, int r);

/* Function to sort an array using insertion sort */
void insertionSort(int arr[], int n);

/* Get median value in window */
int medianFliter(int window[], int n);

// A utility function to print array of size n
void printArray(int arr[], int n);

// Function to determine if directory entry is self or parent
bool selfOrParent(struct dirent *dp);

// Counts the number of files that should be run through the filter
int getNumberOfFiles(char *inDir);

// Gets the list of files to be filtered
void getListOfFiles(char *inDir, char *files[]);

#endif