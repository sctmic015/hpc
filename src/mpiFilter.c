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

void filterImage(char filePath[], char outputPath[], char fileName[], long windowSize, int rank, int nprocs)
{
    // Create combined input and output names
    char *combined = createCombinedName(filePath, fileName);
    char *combinedOut = NULL;

    combinedOut = createCombinedName(outputPath, fileName);

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

        long long update = 0;

        // Loop to paralelise

        unsigned char *output = (unsigned char *)malloc(width * height * 3 * sizeof(unsigned char));
        int counter, start, stop, nloops, finalStart, finalStop, mylen;
        unsigned char *outputSubset = NULL;
        bool option = false;

        //Assign Rows
        int* amountPerRow = (int *) malloc(nprocs * sizeof(int));
        int* rows = (int *) malloc(nprocs * sizeof(int));

        int initialSize = height / nprocs;
        int remainder = height % nprocs;
        if (remainder == 0){
            option = true;
        }
        for (int i = 0; i < nprocs; i ++){
            amountPerRow[i] = initialSize;
            if (remainder != 0){
                amountPerRow[i] ++;
                remainder --;
            }
        }
        rows[0] = amountPerRow[0];
        for (int i = 1; i < nprocs; i ++){
            rows[i] = rows[i-1] + amountPerRow[i];
            //printf("Row Sum rank %d sum %d \n", rank, rows[i]);
        }

        //         for (int i = 0; i < nprocs; i ++){
        //     printf("Row Sum rank %d sum %d \n", rank, amountPerRow[i]);
        // }
        //MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0){
            start = 0;
            stop = rows[0];
            //printf("rank %d start %d stop %d \n", rank, start, stop);
            outputSubset = (unsigned char *)malloc(width * amountPerRow[0] * 3 * sizeof(unsigned char));
            mylen = amountPerRow[0]*width*3;
        }
        else{
            start = rows[rank - 1];
            stop = rows[rank];
            //printf("rank %d start %d stop %d \n", rank, start, stop);
            outputSubset = (unsigned char *)malloc(width * amountPerRow[rank] * 3 * sizeof(unsigned char));
            mylen = amountPerRow[rank]*width*3;
        }
        nloops = 0;
        for (int i = start; i < stop; i++)
        {
            ++nloops;
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

                int countVal = (i-start) * width * 3 + j * 3;
                outputSubset[countVal] = (char)medianFliter(windowR, windowSize * windowSize);
                outputSubset[countVal + 1] = (char)medianFliter(windowG, windowSize * windowSize);
                outputSubset[countVal + 2] = (char)medianFliter(windowB, windowSize * windowSize);
            }
        }

        //printf("Procees %d performed %d iterations pof the loop. \n", rank, nloops);
        free(r);
        free(g);
        free(b);

        if (option){
            MPI_Gather(outputSubset, (width*height*3)/(nprocs), MPI_UNSIGNED_CHAR, output, (width*height*3)/(nprocs), MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            //printf("Ideal Case");
        }
        else {
            int *displs = NULL;
            int *recCounts = NULL;

            if (rank == 0){
                displs = malloc(nprocs * sizeof(int));
                recCounts = malloc(nprocs * sizeof(int));
                displs[0] = 0;
                recCounts[0] = (width*amountPerRow[0]*3);
                //printf("Displacement rank %d = %d and counter = %d\n", rank, displs[rank], counter);
                //printf("RecCounts rank %d = %d and counter = %d\n", rank, recCounts[rank], counter);
                for (int i = 1; i < nprocs; i ++){
                    displs[i] = displs[i-1]+recCounts[i-1];
                    recCounts[i] = width * amountPerRow[i]*3;
                    //printf("Displacement rank %d = %d and counter = %d\n", i, displs[i], counter);
                    //printf("RecCounts rank %d = %d and counter = %d\n", i, recCounts[i], counter);

                    // if (i == nprocs-1){
                    //     displs[i] = i * (width*counter*3);
                    //     recCounts[i] = width * (finalStop - finalStart) * 3;
                    //     printf("Displacement rank %d = %d and counter = %d\n", i, displs[i], counter);
                    //     printf("RecCounts rank %d = %d and counter = %d\n", i, recCounts[i], counter);
                    // }
                    // else {
                    //     displs[i] = i * (width*counter*3);
                    //     recCounts[i] = width * counter * 3;
                    //     printf("Displacement rank %d = %d and counter = %d\n", i, displs[i], counter);
                    //     printf("RecCounts rank %d = %d and counter = %d\n", i, recCounts[i], counter);
                    // }
                }

            }
            //MPI_Barrier(MPI_COMM_WORLD);
            //exit(1);
            // else if (rank!= nprocs -1){
            //     displs[rank] = rank * (width*counter*3);
            //     recCounts[rank] = width * counter * 3;

            //     printf("Displacement rank %d = %d and counter = %d\n", rank, displs[rank], counter);
            //     printf("RecCounts rank %d = %d and counter = %d\n", rank, recCounts[rank], counter);
            // }
            // else {
            //     displs[rank] = rank * (width*counter*3);
            //     printf("Displacement rank %d = %d and counter = %d\n", rank, displs[rank], counter);
            //     recCounts[rank] = width * (finalStop - finalStart) * 3;
            //     printf("RecCounts rank %d = %d and counter = %d\n", rank, recCounts[rank], counter);
            // }
            //MPI_Barrier(MPI_COMM_WORLD);
            MPI_Gatherv(outputSubset, mylen, MPI_UNSIGNED_CHAR, output, recCounts, displs,MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            free(displs);
            free(recCounts);
        }
        //MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0){
            stbi_write_jpg(combinedOut, width, height, 3, output, 50);
        }
        //MPI_Barrier(MPI_COMM_WORLD);
        stbi_image_free(img);
        free(combinedOut);
        free(output);
        free(outputSubset);
        free(amountPerRow);
        free(rows);
        //printf("Rank Test %d \n", rank);

        //printf("%s %d\n", "Done Image. Rank = ", rank);
    }
    else
    {
        //printf("Error in loading the image\n");
    }
}

int main(int argc, char *argv[])
{


    if (isValidArguments(argc, argv))
    {
        // Setup MPI
        double start, end;
        int rank, nprocs;
        int *sendcounts;
        int *displs;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //printf("%d\n",rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
        //printf("%d\n", nprocs);
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0){
            //MPI_Barrier(MPI_COMM_WORLD);
            start = MPI_Wtime();
        }

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
            //printf("%s \n", files[i]);
            //MPI_Barrier(MPI_COMM_WORLD);
            filterImage(inDir, outDir, files[i], windowSize, rank, nprocs);
            //MPI_Barrier(MPI_COMM_WORLD);
        }

        for (int i = 0; i < count; i++)
        {
            free(files[i]);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0){
            //MPI_Barrier(MPI_COMM_WORLD);
            end = MPI_Wtime();
            printf("%f\n", end - start);
        }

        MPI_Finalize();
    }
    else
    {
        quitProgram();
    }
}