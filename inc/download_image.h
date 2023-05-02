#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "ascii_conversion.h"
#include "stb_image.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
Bitmap* download_image(char* url);
Bitmap* download_image_by_id(char* id);
Bitmap* uchar_to_bitmap(unsigned char* data, int width, int height);

