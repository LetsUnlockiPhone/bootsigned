			/*
 * restore.h
 * Functions for handling idevices in restore mode
 *
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */

#ifndef IDEVICERESTORE_RESTORE_H
#define IDEVICERESTORE_RESTORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <plist/plist.h>
#include <libimobiledevice/restore.h>
#include <libimobiledevice/libimobiledevice.h>

struct restore_client_t {
	plist_t tss;
	idevice_t device;
	char* uuid;
	unsigned int operation;
	const char* filesystem;
	uint64_t protocol_version;
	restored_client_t client;
};

int restore_check_mode(const char* uuid);
int restore_check_device(const char* uuid);
int restore_client_new(struct idevicerestore_client_t* client);
void restore_client_free(struct idevicerestore_client_t* client);
int restore_reboot(struct idevicerestore_client_t* client);
const char* restore_progress_string(unsigned int operation);
int restore_handle_status_msg(restored_client_t client, plist_t msg);
int restore_handle_progress_msg(restored_client_t client, plist_t msg);
int restore_handle_data_request_msg(struct idevicerestore_client_t* client, idevice_t device, restored_client_t restore, plist_t message, plist_t build_identity, const char* filesystem);
int restore_send_nor(restored_client_t restore, struct idevicerestore_client_t* client, plist_t build_identity);
int restore_send_root_ticket(restored_client_t restore, struct idevicerestore_client_t* client);
int restore_send_kernelcache(restored_client_t restore, struct idevicerestore_client_t* client, plist_t build_identity);
int restore_device(struct idevicerestore_client_t* client, plist_t build_identity, const char* filesystem);
int restore_open_with_timeout(struct idevicerestore_client_t* client);
int restore_send_filesystem(idevice_t device, const char* filesystem);


#ifdef __cplusplus
}
#endif

#endif
