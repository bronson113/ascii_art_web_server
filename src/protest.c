#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

unsigned char* step2(char* url) {
	CURL *curl_handle;
	CURLcode res;
	FILE *fp;
	//sets filename for downloading image
	char outfilename[FILENAME_MAX] = "temp.jpg";
	
	//initializes the CURL handle for url request
	curl_handle = curl_easy_init();
	if (curl_handle) {
		//set image url to download
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		//open file fo writng downloaded file
		fp = fopen(outfilename, "wb");
		if (fp == NULL) {
			printf("Failed to open file\n");
			return NULL;
		}
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl_handle);
		curl_easy_cleanup(curl_handle);
		fclose(fp);
	}

	int width, height, channels;
	unsigned char* image_data = stbi_load(outfilename, &width, &height, &channels, STBI_rgb_alpha);
	if (image_data == NULL) {
		printf("Error loading image\n");
		return NULL;
	}
	return image_data;
}

