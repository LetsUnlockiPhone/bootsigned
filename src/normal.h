/*
 * normal.h
 * Functions for handling idevices in normal mode
 *
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>

#ifndef IDEVICERESTORE_NORMAL_H
#define IDEVICERESTORE_NORMAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/libimobiledevice.h>

struct normal_client_t {
	idevice_t device;
	lockdownd_client_t client;
	const char* ipsw;
	plist_t tss;
};


int normal_check_mode(struct idevicerestore_client_t* client);
int normal_check_device(struct idevicerestore_client_t* client);
int normal_client_new(struct idevicerestore_client_t* client);
void normal_client_free(struct idevicerestore_client_t* client);
int normal_open_with_timeout(struct idevicerestore_client_t* client);
int normal_enter_recovery(struct idevicerestore_client_t* client);
int normal_get_cpid(struct idevicerestore_client_t* client, uint32_t* cpid);
int normal_get_bdid(struct idevicerestore_client_t* client, uint32_t* bdid);
int normal_get_ecid(struct idevicerestore_client_t* client, uint64_t* ecid);

#ifdef __cplusplus
}
#endif

#endif
