/**
  * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 **/

#ifndef LIBIRECOVERY_H
#define LIBIRECOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#include <libusb-1.0/libusb.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef interface
#ifndef sleep
#define sleep(n) Sleep(1000 * n)
#endif
#endif

#define APPLE_VENDOR_ID 0x05AC

#define CPID_UNKNOWN          -1
#define CPID_IPHONE2G     0x8900
#define CPID_IPOD1G       0x8900
#define CPID_IPHONE3G     0x8900
#define CPID_IPOD2G       0x8720
#define CPID_IPHONE3GS    0x8920
#define CPID_IPOD3G       0x8922
#define CPID_IPAD1G       0x8930
#define CPID_IPHONE4      0x8930
#define CPID_IPOD4G       0x8930
#define CPID_APPLETV2     0x8930
#define CPID_IPHONE42     0x8930
#define CPID_IPAD21       0x8940
#define CPID_IPAD22       0x8940
#define CPID_IPAD23       0x8940
#define CPID_IPHONE4S     0x8940

#define BDID_UNKNOWN          -1
#define BDID_IPHONE2G       0x00
#define BDID_IPOD1G         0x02
#define BDID_IPHONE3G       0x04
#define BDID_IPOD2G         0x00
#define BDID_IPHONE3GS      0x00
#define BDID_IPOD3G         0x02
#define BDID_IPAD1G         0x02
#define BDID_IPHONE4        0x00
#define BDID_IPOD4G         0x08
#define BDID_APPLETV2       0x10
#define BDID_IPHONE42       0x06
#define BDID_IPAD21         0x04
#define BDID_IPAD22         0x06
#define BDID_IPAD23         0x02
#define BDID_IPHONE4S       0x08

#define DEVICE_UNKNOWN      -1
#define DEVICE_IPHONE2G      0
#define DEVICE_IPOD1G        1
#define DEVICE_IPHONE3G      2
#define DEVICE_IPOD2G        3
#define DEVICE_IPHONE3GS     4
#define DEVICE_IPOD3G        5
#define DEVICE_IPAD1G        6
#define DEVICE_IPHONE4       7
#define DEVICE_IPOD4G        8
#define DEVICE_APPLETV2      9
#define DEVICE_IPHONE42     10
#define DEVICE_IPAD21       11
#define DEVICE_IPAD22       12
#define DEVICE_IPAD23       13
#define DEVICE_IPHONE4S     14

enum {
	kRecoveryMode1 = 0x1280,
	kRecoveryMode2 = 0x1281,
	kRecoveryMode3 = 0x1282,
	kRecoveryMode4 = 0x1283,
	kWTFMode = 0x1222,
	kDfuMode = 0x1227
};

typedef enum {
	IRECV_E_SUCCESS = 0,
	IRECV_E_NO_DEVICE = -1,
	IRECV_E_OUT_OF_MEMORY = -2,
	IRECV_E_UNABLE_TO_CONNECT = -3,
	IRECV_E_INVALID_INPUT = -4,
	IRECV_E_FILE_NOT_FOUND = -5,
	IRECV_E_USB_UPLOAD = -6,
	IRECV_E_USB_STATUS = -7,
	IRECV_E_USB_INTERFACE = -8,
	IRECV_E_USB_CONFIGURATION = -9,
	IRECV_E_PIPE = -10,
	IRECV_E_TIMEOUT = -11,
	IRECV_E_UNKNOWN_ERROR = -255
} irecv_error_t;

typedef enum {
	IRECV_RECEIVED = 1,
	IRECV_PRECOMMAND = 2,
	IRECV_POSTCOMMAND = 3,
	IRECV_CONNECTED = 4,
	IRECV_DISCONNECTED = 5,
	IRECV_PROGRESS = 6
} irecv_event_type;

typedef struct {
	int size;
	char* data;
	double progress;
	irecv_event_type type;
} irecv_event_t;

struct irecv_client;
typedef struct irecv_client* irecv_client_t;
typedef struct irecv_device* irecv_device_t;
typedef int(*irecv_event_cb_t)(irecv_client_t client, const irecv_event_t* event);

struct irecv_client {
	int debug;
	int config;
	int interface;
	int alt_interface;
	unsigned short mode;
	char serial[256];
	
#ifndef WIN32
	libusb_device_handle* handle;
#else
	HANDLE handle;
	HANDLE hDFU;
	HANDLE hIB;
	LPSTR iBootPath;
	LPSTR DfuPath;
#endif
	
	irecv_event_cb_t progress_callback;
	irecv_event_cb_t received_callback;
	irecv_event_cb_t connected_callback;
	irecv_event_cb_t precommand_callback;
	irecv_event_cb_t postcommand_callback;
	irecv_event_cb_t disconnected_callback;
};

struct irecv_device {
	int index;
	const char* product;
	const char* model;
	unsigned int board_id;
	unsigned int chip_id;
};

static struct irecv_device irecv_devices[] = {
	{  0, "iPhone1,1",  "m68ap", 0x00, 0x8900 },
	{  1, "iPod1,1",    "n45ap", 0x02, 0x8900 },
	{  2, "iPhone1,2",  "n82ap", 0x04, 0x8900 },
	{  3, "iPod2,1",    "n72ap", 0x00, 0x8720 },
	{  4, "iPhone2,1",  "n88ap", 0x00, 0x8920 },
	{  5, "iPod3,1",    "n18ap", 0x02, 0x8922 },
	{  6, "iPad1,1",    "k48ap", 0x02, 0x8930 },
	{  7, "iPhone3,1",  "n90ap", 0x00, 0x8930 },
	{  8, "iPod4,1",    "n81ap", 0x08, 0x8930 },
	{  9, "AppleTV2,1", "k66ap", 0x10, 0x8930 },
	{ 10, "iPhone3,3",  "n92ap", 0x06, 0x8930 },
	{ 11, "iPad2,1",    "k93ap", 0x04, 0x8940 },
	{ 12, "iPad2,2",    "k94ap", 0x06, 0x8940 },
	{ 13, "iPad2,3",    "k95ap", 0x02, 0x8940 },
	{ 14, "iPhone4,1",  "n94ap", 0x08, 0x8940 },
	{ -1,  NULL,           NULL, -1,    -1 }
};

void irecv_set_debug_level(int level);
const char* irecv_strerror(irecv_error_t error);
irecv_error_t irecv_open_attempts(irecv_client_t* pclient, unsigned long long ecid, int attempts);
irecv_error_t irecv_open(irecv_client_t* client, unsigned long long ecid);
irecv_error_t irecv_reset(irecv_client_t client);
irecv_error_t irecv_close(irecv_client_t client);
irecv_error_t irecv_receive(irecv_client_t client);
irecv_error_t irecv_send_exploit(irecv_client_t client);
irecv_error_t irecv_execute_script(irecv_client_t client, const char* filename);
irecv_error_t irecv_set_configuration(irecv_client_t client, int configuration);

irecv_error_t irecv_event_subscribe(irecv_client_t client, irecv_event_type type, irecv_event_cb_t callback, void *user_data);
irecv_error_t irecv_event_unsubscribe(irecv_client_t client, irecv_event_type type);

irecv_error_t irecv_send_file(irecv_client_t client, const char* filename, int dfuNotifyFinished);
irecv_error_t irecv_send_command(irecv_client_t client, char* command);
irecv_error_t irecv_send_buffer(irecv_client_t client, unsigned char* buffer, unsigned long length, int dfuNotifyFinished);

irecv_error_t irecv_saveenv(irecv_client_t client);
irecv_error_t irecv_getret(irecv_client_t client, unsigned int* value);
irecv_error_t irecv_getenv(irecv_client_t client, const char* variable, char** value);
irecv_error_t irecv_setenv(irecv_client_t client, const char* variable, const char* value);
irecv_error_t irecv_set_interface(irecv_client_t client, int interface, int alt_interface);
irecv_error_t irecv_get_cpid(irecv_client_t client, unsigned int* cpid);
irecv_error_t irecv_get_bdid(irecv_client_t client, unsigned int* bdid);
irecv_error_t irecv_get_ecid(irecv_client_t client, unsigned long long* ecid);
irecv_error_t irecv_get_nonce(irecv_client_t client, unsigned char** nonce, int* nonce_size);
void irecv_hexdump(unsigned char* buf, unsigned int len, unsigned int addr);

void irecv_init();
void irecv_exit();
irecv_client_t irecv_reconnect(irecv_client_t client, int initial_pause);
irecv_error_t irecv_reset_counters(irecv_client_t client);
irecv_error_t irecv_finish_transfer(irecv_client_t client);
irecv_error_t irecv_recv_buffer(irecv_client_t client, char* buffer, unsigned long length);
irecv_error_t irecv_get_device(irecv_client_t client, irecv_device_t* device);

#ifdef __cplusplus
}
#endif

#endif
