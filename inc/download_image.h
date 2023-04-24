#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ascii_conversion.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
Bitmap* download_image(char* url);
Bitmap* uchar_to_bitmap(unsigned char* data, int width, int height);
