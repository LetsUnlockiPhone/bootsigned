/*
 * ipsw.c
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */

#ifndef IDEVICERESTORE_IPSW_H
#define IDEVICERESTORE_IPSW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zip.h>
#include <stdint.h>
#include <plist/plist.h>

typedef struct {
	int index;
	char* name;
	unsigned int size;
	unsigned char* data;
} ipsw_file;

int ipsw_extract_to_memory(const char* ipsw, const char* infile, char** pbuffer, uint32_t* psize);
int ipsw_extract_build_manifest(const char* ipsw, plist_t* buildmanifest, int *tss_enabled);
int ipsw_extract_restore_plist(const char* ipsw, plist_t* restore_plist);
void ipsw_free_file(ipsw_file* file);

#ifdef __cplusplus
}
#endif

#endif
