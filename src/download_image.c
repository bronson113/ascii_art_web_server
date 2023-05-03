#include "download_image.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}
Bitmap* download_image(char* url){
	static int serial_number = 0;
	CURL *curl_handle;
	CURLcode res;
	FILE *fp;
	//sets filename for downloading image
	char outfilename[FILENAME_MAX];
	printf("Actual url: %s\n", url);
	sprintf(outfilename, "/tmp/image%016d.jpg", getpid()*10000 + serial_number++);
	
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
		printf("Successfully opened file\n");

		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl_handle);
		curl_easy_cleanup(curl_handle);
		fclose(fp);
	}

	int width, height, channels;
	unsigned char* image_data = stbi_load(outfilename, &width, &height, &channels, STBI_rgb);
	if (image_data == NULL) {
		printf("Error loading image\n");
		return NULL;
	}
	printf("successfully loaded image\n");
	Bitmap* ret = uchar_to_bitmap(image_data, width, height);
	free(image_data);
	return ret;
}

Bitmap* uchar_to_bitmap(unsigned char* image_data, int width, int height) {
	Bitmap* ret = malloc(sizeof(Bitmap));
	if(ret == NULL){
		printf("Error allocating memory for bitmap\n");
		return NULL;
	}
	ret->Width = width;
	ret->Height = height;
	ret->Pixels = malloc(ret->Height * sizeof(Color*));
	for(int i=0;i<height;i++){
		ret->Pixels[i] = malloc(ret->Width * sizeof(Color));
		for(int j=0;j<width;j++){
			ret->Pixels[i][j].R = image_data[i*3*width+j*3];
			ret->Pixels[i][j].G = image_data[i*3*width+j*3+1];
			ret->Pixels[i][j].B = image_data[i*3*width+j*3+2];
		}
	}
	return ret;
}

Bitmap* download_image_by_id(char* id)
{
	char url[] = "https://i.imgur.com/";
	char extention[] = ".jpeg";
	char target_url[200];
	memset(target_url, 0, sizeof(target_url));
	sprintf(target_url, "%s%s%s", url, id, extention);
	printf("Actual location: %s\n", target_url);
	return download_image(target_url);
}
