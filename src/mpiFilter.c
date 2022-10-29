/**
 * @author Michael Scott
 * @author Bradley Culligan
 * @author Jared May
 * @version October 2022
 *
 * Main launching point of median filter program for MPI solution
 */

// Headers
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <dirent.h>

#include <mpi.h>

#include "argumentChecker.h"
#include "utilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void filterImage(char filePath[], char outputPath[], char fileName[], long windowSize)
{
    // Create combined input and output names
    char *combined = createCombinedName(filePath, fileName);
    char *combinedOut = createCombinedName(outputPath, fileName);

    // Load image
    int width, height, bpp;
    unsigned char *img = stbi_load(combined, &width, &height, &bpp, 3);
    free(combined);

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
        long windowOffset = (0.5) * windowSize - 0.5;
        int imageSize = (int)windowSize * windowSize;
        unsigned char *output = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
        long long update = 0;

        //#pragma omp parallel for
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
        free(combinedOut);
        free(output);

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
        // Setup MPI
        int rank, size;
        int *sendcounts;
        int *displs;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        // Skip the executable name
        --argc;
        ++argv;

        char *inDir = argv[0];
        char *outDir = argv[1];
        int windowSize = atoi(argv[2]);

        int count = getNumberOfFiles(inDir);

        // Further MPI setup
        int minNumElements = count / size;
        int remainder = count % size;
        int k = 0;
        sendcounts = malloc(sizeof(int) * size);
        displs = malloc(sizeof(int) * size);

        // Get distribution of values
        for (int i = 0; i < size; i++)
        {
            sendcounts[i] = minNumElements;

            if (i < remainder)
            {
                sendcounts[i]++;
            }

            displs[i] = k;
            k += sendcounts[i];
        }

        // Populate files Array
        char *files[count];
        getListOfFiles(inDir, files);

        // TODO: FIX Define file subset array of files for each process and scatter files appropriately
        char *fileSubset[sendcounts[rank]];
        // TODO: SCATTERV FILES INTO FILESUBSET THAT EACH PROC HANDLES

        for (int i = 0; i < sendcounts[rank]; i++)
        {
            printf("Process %d: %s\n", rank, fileSubset[i]);
            filterImage(inDir, outDir, fileSubset[i], windowSize);
        }

        MPI_Finalize();
        free(sendcounts);
        free(displs);

        for (int i = 0; i < count; i++)
        {
            free(files[i]);
        }

        // stop = time(NULL);
        // printf("Run Time: %ld\n", stop - start);
    }
    else
    {
        quitProgram();
    }
}
