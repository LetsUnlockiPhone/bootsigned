/*
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */

#ifndef IDEVICERESTORE_DFU_H
#define IDEVICERESTORE_DFU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libirecovery.h>
#include "common.h"

struct dfu_client_t {
	irecv_client_t client;
	const char* ipsw;
	plist_t tss;
};

int dfu_client_new(struct idevicerestore_client_t* client);
void dfu_client_free(struct idevicerestore_client_t* client);
int dfu_check_mode(struct idevicerestore_client_t* client, int* mode);
int dfu_send_buffer(struct idevicerestore_client_t* client, char* buffer, uint32_t size);
int dfu_send_component(struct idevicerestore_client_t* client, plist_t build_identity, const char* component);
int dfu_get_cpid(struct idevicerestore_client_t* client, unsigned int* cpid);
int dfu_get_nonce(struct idevicerestore_client_t* client, unsigned char** nonce, int* nonce_size);
int dfu_enter_recovery(struct idevicerestore_client_t* client, plist_t build_identity);


#ifdef __cplusplus
}
#endif

#endif
