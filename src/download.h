/*
 * download.h
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>              
 */
#ifndef IDEVICERESTORE_DOWNLOAD_H
#define IDEVICERESTORE_DOWNLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int download_to_buffer(const char* url, char** buf, uint32_t* length);
int download_to_file(const char* url, const char* filename);

#ifdef __cplusplus
}
#endif

#endif
