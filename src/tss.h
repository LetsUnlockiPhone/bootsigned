/*
 * ipsw.c
 * Definitions for communicating with Apple's TSS server.
 *
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */

#ifndef IDEVICERESTORE_TSS_H
#define IDEVICERESTORE_TSS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <plist/plist.h>

plist_t tss_send_request(plist_t request);
plist_t tss_create_request(plist_t build_identity, uint64_t ecid, unsigned char* nonce, int nonce_size);
int tss_get_ticket(plist_t tss, unsigned char** ticket, uint32_t* tlen);
int tss_get_entry_path(plist_t tss, const char* entry, char** path);
int tss_get_blob_by_path(plist_t tss, const char* path, char** blob);
int tss_get_blob_by_name(plist_t tss, const char* entry, char** blob);


#ifdef __cplusplus
}
#endif

#endif
