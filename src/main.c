/**
 * @author Michael Scott
 * @author Bradley Culligan
 * @author Jared May
 * @version October 2022
 *
 * Main launching point of median filter program
 */

// Headers
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define _DEFAULT_SOURCE
#include <dirent.h>

#include <omp.h>

#include "argumentChecker.h"
#include "utilities.h"

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

void filterImage(char filePath[], char outputPath[], char fileName[], long windowSize)
{
    // Load File Path
    char *combined = malloc(strlen(filePath) + strlen(fileName));
    strcpy(combined, filePath);
    strcat(combined, "/");
    strcat(combined, fileName);

    // Out file Path
    char *combinedOut = malloc(strlen(outputPath) + strlen(fileName));
    strcpy(combinedOut, outputPath);
    strcat(combinedOut, "/");
    strcat(combinedOut, fileName);
    int width, height, bpp;

    unsigned char *img = stbi_load(combined, &width, &height, &bpp, 3);

    if (img != NULL)
    {
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
    else
    {
        printf("Error in loading the image\n");
    }
}

int main(int argc, char *argv[])
{
    // time_t start, stop;
    // start = time(NULL);

    if (isValidArguments(argc, argv))
    {
        // Skip the executable name
        --argc;
        ++argv;

        char *inDir = argv[0];
        char *outDir = argv[1];
        int windowSize = atoi(argv[2]);

        int count = getNumberOfFiles(inDir);

        // Populate files Array
        char *files[count];
        getListOfFiles(inDir, files);

        for (int i = 0; i < count; i++)
        {
            printf("%s\n", files[i]);
            filterImage(inDir, outDir, files[i], windowSize);
        }

        // stop = time(NULL);
        // printf("Run Time: %ld\n", stop - start);
    }
    else
    {
        quitProgram();
    }
}
