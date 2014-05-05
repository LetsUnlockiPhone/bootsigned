/*
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */

#ifndef IDEVICERESTORE_ASR_H
#define IDEVICERESTORE_ASR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libimobiledevice/libimobiledevice.h>

int asr_open_with_timeout(idevice_t device, idevice_connection_t* asr);
int asr_send(idevice_connection_t asr, plist_t* data);
int asr_receive(idevice_connection_t asr, plist_t* data);
int asr_send_buffer(idevice_connection_t asr, const char* data, uint32_t size);
void asr_close(idevice_connection_t asr);
int asr_perform_validation(idevice_connection_t asr, const char* filesystem);
int asr_send_payload(idevice_connection_t asr, const char* filesystem);
int asr_handle_oob_data_request(idevice_connection_t asr, plist_t packet, FILE* file);


#ifdef __cplusplus
}
#endif

#endif
