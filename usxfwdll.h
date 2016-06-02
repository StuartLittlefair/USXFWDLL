#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

// VendorID and ProductID for USX FW
extern unsigned short VID;
extern unsigned short PID;

void sleep(int sleepMS);

int initialise();

int finalise();

hid_device* open_device();

void close_device(hid_device* handle);

int wait_and_read(hid_device* handle, unsigned char* buf);

int request_filter(hid_device* handle, int filter_num);

int check_curr_filter(hid_device* handle);