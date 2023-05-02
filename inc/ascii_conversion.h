#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
credit to bitesofcode.com for the concept and ChatGPT for the generation of the code. Comments are utilized here to indicate
understanding of the code
*/

//defines a structure to store the RGB values for the bitmap 
typedef struct
{
    int R;
    int G;
    int B;
} Color;

//defines a structure that defines a Bitmap object and different objects within 
typedef struct
{
    int Width;
    int Height;
    Color** Pixels;
} Bitmap;

typedef struct
{
    int Width;
    int Height;
    char** Pixels;
} Asciimap;

Bitmap Grayscale(Bitmap image);
Asciimap Convert_to_ascii(Bitmap image);