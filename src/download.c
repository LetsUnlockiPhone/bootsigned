/*
 * download.c
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "download.h"
#include "common.h"

#ifdef WIN32
#define ftello(x) ftell(x)
#endif

typedef struct {
	int length;
	char* content;
} curl_response;

static size_t download_write_buffer_callback(char* data, size_t size, size_t nmemb, curl_response* response) {
	size_t total = size * nmemb;
	if (total != 0) {
		response->content = realloc(response->content, response->length + total + 1);
		memcpy(response->content + response->length, data, total);
		response->content[response->length + total] = '\0';
		response->length += total;
	}
	return total;
}

int download_to_buffer(const char* url, char** buf, uint32_t* length)
{
	int res = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* handle = curl_easy_init();
	if (handle == NULL) {
		error("ERROR: could not initialize CURL\n");
		return -1;
	}

	curl_response response;
	response.length = 0;
	response.content = malloc(1);
	response.content[0] = '\0';

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, (curl_write_callback)&download_write_buffer_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "InetURL/1.0");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(handle, CURLOPT_URL, url);

	curl_easy_perform(handle);
	curl_easy_cleanup(handle);

	if (response.length > 0) {
		*length = response.length;
		*buf = response.content;
	} else {
		res = -1;
	}

	curl_global_cleanup();

	return res;
}

int download_progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	double p = (dlnow / dltotal) * 100;

	if (p < 100.0f) {
		print_progress_bar(p);
	}

	return 0;
}

int download_to_file(const char* url, const char* filename)
{
	int res = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* handle = curl_easy_init();
	if (handle == NULL) {
		error("ERROR: could not initialize CURL\n");
		return -1;
	}

	FILE* f = fopen(filename, "wb");
	if (!f) {
		error("ERROR: cannot open '%s' for writing\n", filename);
		return -1;
	}

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, (curl_write_callback)&fwrite);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, (curl_progress_callback)&download_progress);
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "InetURL/1.0");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(handle, CURLOPT_URL, url);

	curl_easy_perform(handle);
	curl_easy_cleanup(handle);

	off_t sz = ftello(f);
	fclose(f);

	if ((sz == 0) || (sz == (off_t)-1)) {
		res = -1;
		remove(filename);
	}

	curl_global_cleanup();

	return res;
}
