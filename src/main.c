#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define _DEFAULT_SOURCE
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// unsigned char readImage(char imagename[]){
//     int width, height, bpp;
//
//     uint8_t *img = stbi_load("C:\\Users\\micha\\CLionProjects\\HPC\\1.jpeg", &width, &height, &bpp, 4);
//
//     return img
//     stbi_image_free(img);
// }

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

void filterImage(char filePath[], char outputPath[], char fileName[], long windowSize)
{
    // Load File Path
    char *combined = malloc(strlen(filePath) + strlen(fileName));
    strcpy(combined, filePath);
    strcat(combined, fileName);

    // Out file Path
    char *combinedOut = malloc(strlen(outputPath) + strlen(fileName));
    strcpy(combinedOut, outputPath);
    strcat(combinedOut, fileName);
    int width, height, bpp;

    unsigned char *img = stbi_load(combined, &width, &height, &bpp, 3);

    if (img == NULL)
    {
        printf("Error in loading the image\n");
        // exit(1);
    }

    unsigned bytePerPixel = 3;
    int *r = (int *)malloc(width * height * sizeof(int));
    int *g = (int *)malloc(width * height * sizeof(int));
    int *b = (int *)malloc(width * height * sizeof(int));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            unsigned char *pixelOffset = img + (j + height * i) * bytePerPixel; //(column _ width * row)
            *(r + i * height + j) = pixelOffset[0];
            *(g + i * height + j) = pixelOffset[1];
            *(b + i * height + j) = pixelOffset[2];
        }
    }

    // Implement median filter
    // long windowSize = 3;
    long windowOffset = (0.5) * windowSize - 0.5;
    int imageSize = (int)windowSize * windowSize;
    unsigned char *output = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
    long long update = 0;
// int count = 0;
#pragma omp parallel for
    for (int i = 0; i < height; i++)
    {
        int windowR[imageSize];
        int windowG[imageSize];
        int windowB[imageSize];
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            int kcopy;
            int lcopy;
            for (int k = i - windowOffset; k <= i + windowOffset; k++)
            {
                for (int l = j - windowOffset; l <= j + windowOffset; l++)
                {
                    kcopy = abs(k);
                    lcopy = abs(l);
                    if (k > height - 1)
                    {
                        kcopy = (height - 1) - (kcopy - (height - 1));
                    }
                    if (l > width - 1)
                    {
                        lcopy = (width - 1) - (lcopy - (width - 1));
                    }
                    windowR[count] = *(r + kcopy * width + lcopy);
                    windowG[count] = *(g + kcopy * width + lcopy);
                    windowB[count] = *(b + kcopy * width + lcopy);
                    count++;
                }
            }

            int countVal = i * width * 3 + j * 3;
            output[countVal] = (char)medianFliter(windowR, windowSize * windowSize);
            output[countVal + 1] = (char)medianFliter(windowG, windowSize * windowSize);
            output[countVal + 2] = (char)medianFliter(windowB, windowSize * windowSize);
        }
    }

    free(r);
    free(g);
    free(b);

    stbi_write_jpg(combinedOut, width, height, 3, output, 50);
    stbi_image_free(img);

    printf("%s\n", "Done Image");
}

int main(int argc, char *argv[])
{
    time_t start, stop;
    start = time(NULL);
    char *inputFolder = argv[1];
    char *outputFolder = argv[2];
    long windowSize = atoi(argv[3]);
    if (windowSize > 25 || windowSize < 3 || windowSize % 2 == 0)
    {
        printf("Incorrect Window Size");
        exit(1);
    }

    // Attempt to try and do static array
    // Count number of files
    int count = 0;
    struct dirent *dp;
    DIR *dir;
    if ((dir = opendir("C:/Users/micha/CLionProjects/HPC/Images")) == NULL)
    {
        fprintf(stderr, "listdir: can't open %s\n", dir);
    }
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
            continue; /* skip self and parent */
        printf("%s/%s\n", dir, dp->d_name);
        count++;
    }
    closedir(dir);

    // Populate files Array
    char *files[count];
    struct dirent *dp2;
    DIR *dir2;
    if ((dir2 = opendir("C:/Users/micha/CLionProjects/HPC/Images")) == NULL)
    {
        fprintf(stderr, "listdir: can't open %s\n", dir2);
    }
    int fileNum = 0;
    while ((dp2 = readdir(dir2)) != NULL)
    {
        if (!strcmp(dp2->d_name, ".") || !strcmp(dp2->d_name, ".."))
            continue; /* skip self and parent */
        files[fileNum] = (char *)malloc(strlen(dp2->d_name) + 1);
        strcpy(files[fileNum], dp2->d_name);
        fileNum++;
        // printf(file);
    }
    closedir(dir2);

    for (int i = 0; i < count; i++)
    {
        printf("%s\n", files[i]);
        filterImage(inputFolder, outputFolder, files[i], windowSize);
    }

    stop = time(NULL);
    printf("Run Time: %ld\n", stop - start);
    return 0;
}
