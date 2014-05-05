/*
 * recovery.h
 * Functions for handling idevices in recovery mode
 *
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>

#ifndef IDEVICERESTORE_RECOVERY_H
#define IDEVICERESTORE_RECOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <plist/plist.h>

#include "common.h"

#include "libirecovery.h"

//struct irecv_client;
//typedef struct irecv_client* irecv_client_t;
struct recovery_client_t {
	irecv_client_t client;
	const char* ipsw;
	plist_t tss;
};

int recovery_check_mode(struct idevicerestore_client_t* client);
int recovery_client_new(struct idevicerestore_client_t* client);
void recovery_client_free(struct idevicerestore_client_t* client);
int recovery_send_component(struct idevicerestore_client_t* client, plist_t build_identity, const char* component);
int recovery_send_ibec(struct idevicerestore_client_t* client, plist_t build_identity);
int recovery_send_applelogo(struct idevicerestore_client_t* client, plist_t build_identity);
int recovery_send_devicetree(struct idevicerestore_client_t* client, plist_t build_identity);
int recovery_send_ramdisk(struct idevicerestore_client_t* client, plist_t build_identity);
int recovery_send_kernelcache(struct idevicerestore_client_t* client, plist_t build_identity);
int recovery_send_reset(struct idevicerestore_client_t* client);
int recovery_get_ecid(struct idevicerestore_client_t* client, uint64_t* ecid);
int recovery_get_nonce(struct idevicerestore_client_t* client, unsigned char** nonce, int* nonce_size);
int recovery_get_cpid(struct idevicerestore_client_t* client, uint32_t* cpid);
int recovery_get_bdid(struct idevicerestore_client_t* client, uint32_t* bdid);


#ifdef __cplusplus
}
#endif

#endif
