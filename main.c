#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_master/stb_image_write.h"

//unsigned char readImage(char imagename[]){
//    int width, height, bpp;
//
//    uint8_t *img = stbi_load("C:\\Users\\micha\\CLionProjects\\HPC\\1.jpeg", &width, &height, &bpp, 4);
//
//    return img
//    stbi_image_free(img);
//}

/* Function to sort an array
   using insertion sort*/
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

int medianFliter(int window[], int n){
    insertionSort(window, n);
    int median = window[(n+1)/2];
    return median;
}
// A utility function to print
// an array of size n
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}



int main() {
    int width, height, bpp;

    unsigned char *img = stbi_load("C:\\Users\\micha\\CLionProjects\\HPC\\1.jpeg", &width, &height, &bpp, 3);
    unsigned bytePerPixel = 3;
    for (int i = 0; i < 20; i ++){
        printf("%d ", img[i]);
    }
    int r[width][height];
    int g[width][height];
    int b[width][height];
    for (int i = 0; i < width; i ++){
        for (int j = 0; j < height; j ++){
            unsigned char* pixelOffset = img + (j + width * i) * bytePerPixel;   //(column _ width * row)
            r[i][j] = pixelOffset[0];
            g[i][j] = pixelOffset[1];
            b[i][j] = pixelOffset[2];
//            printf("%d" " " "%d" " " "%d", r, g, b);
//            printf("\n");
        }
    }

    // Implement median filter
    float windowSize = 7;
    float windowOffset =  (0.5)*windowSize - 0.5;
    int imageSize = (int) windowSize * windowSize;
    unsigned char output[width*height*3];
    int update = 0;
    for (int i = 0; i < width; i ++){
        for (int j = 0; j < height; j ++){
            // Create set of images to smooth
            int windowR[imageSize];
            int windowG[imageSize];
            int windowB[imageSize];
            int count = 0;
            for (int k = i - windowOffset; k <= i + windowOffset; k ++){
                for (int l = j - windowOffset; l < j+  windowOffset; l ++){
                    windowR[count] = r[abs(k)][abs(l)];
                    windowG[count] = g[abs(k)][abs(l)];
                    windowB[count] = b[abs(k)][abs(l)];
                    count ++;
                }
            }
            output[update++] = (char) medianFliter(windowR, windowSize * windowSize);
            output[update++] = (char) medianFliter(windowG, windowSize * windowSize);
            output[update++] = (char) medianFliter(windowB, windowSize * windowSize);

        }
    }

    stbi_write_jpg("C:\\Users\\micha\\CLionProjects\\HPC\\1out.jpg", width, height, 3, output, 50);
    stbi_image_free(img);
    return 0;
}
