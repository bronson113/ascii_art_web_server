#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
unsigned char* download_image(char* url);
