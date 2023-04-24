#include <stdio.h>
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


//takes in a bitmap file and converts it to grayscale 
Bitmap Grayscale(Bitmap image)
{
    Bitmap btm;
    btm.Width = image.Width;
    btm.Height = image.Height;
    btm.Pixels = malloc(btm.Width * sizeof(Color*)); //allocates the size of the element 

    for (int i = 0; i < btm.Width; i++) 
    {
        btm.Pixels[i] = malloc(btm.Height * sizeof(Color)); //allocates the size of the element 

        for (int j = 0; j < btm.Height; j++)
        {
            int ser = (int)(image.Pixels[i][j].R * 0.3 + image.Pixels[i][j].G * 0.59 + image.Pixels[i][j].B * 0.11);
            btm.Pixels[i][j].R = ser;
            btm.Pixels[i][j].G = ser;
            btm.Pixels[i][j].B = ser;
        }
    }

    return btm;
}

Asciimap Convert_to_ascii(Bitmap image){
    int block_size = 2;
    Bitmap grey = Grayscale(image);
    Asciimap ret;
    ret.Width = grey.Width/block_size; 
    ret.Height = grey.Height/block_size; 
    ret.Pixels = malloc(ret.Width * sizeof(char));
    for (int n = 0; n < ret.Width; n++) 
    {
        ret.Pixels[n] = malloc(ret.Height * sizeof(char)); //allocates the size of the element 

        for (int m = 0; m < ret.Height; m++)
        {
            int sum = 0; 
            for (int i = 0, i < block_size, i++)
            {
                for (int j = 0, j < block_size, j++)
                {
                    sum += grey[n*block_size+i][m*block_size+j].R;                
                }
            }
            float average = sum / block_size / block_size;
            char mapping[] = "1234567890";
            int index = average / (255.0 / strlen(mapping));
            ret[n][m] = mapping[index];
        }
    }
    return ret;
}

