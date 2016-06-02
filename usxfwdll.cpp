#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255
#define DELAY_MS 2

// VendorID and ProductID for USX FW
unsigned short VID = 0x922;
unsigned short PID = 0x7234;

void sleep(int sleepMS)
{
#ifdef WINDOWS
    Sleep(sleepMS);
#else
    usleep(sleepMS * 1000);
#endif
}

int initialise()
{
    // initialise hidapi library
    return hid_init();
}

int finalize()
{
    // finalize hidapi library
    return hid_exit();
}

hid_device* get_handle()
{
    // open FW device and get handle for read/write
    return hid_open(VID, PID, NULL);
}

int wait_and_read(hid_device* handle, unsigned char* buf)
{
    int res;
    sleep(DELAY_MS);
    res = hid_read(handle, buf, 2);
    if (res > 0){
        return 1;
    } else {
        return 0;
    }
}

int request_filter(int filter_num)
{
    /* Send a command to set the current filter.
    
    Returns immediately, does not block
    */
    int res;
    unsigned char buf[2];
    hid_device *handle;
    
    res = initialise();
    if (res > 1){
        return res;
    }
    
    handle = get_handle();
    if (!handle){
        return 1;
    }
    
    buf[0] = 0x0;
    buf[1] = filter_num;
    res = hid_write(handle, buf, 2);
    if (res > 1){
       return res;
    }
    res = wait_and_read(handle, buf);
    if (res >1 ){
        return res;
    }
    
    // Print out the returned buffer.
    int i;
	for (i = 0; i < 4; i++)
		printf("buf[%d]: %d\n", i, buf[i]);
    
    res = finalize();
    return res;
}

int check_curr_filter()
{
    /* Request the current filter number from the filter wheel.a
    
    May be 0, which indicates filter is still moving.
    */
    int res;
    unsigned char buf[2];
    hid_device *handle;
    
    res = initialise();
    handle = get_handle();

    buf[0] = 0x0;
    buf[1] = 0x0;
    res = hid_write(handle, buf, 2);
    wait_and_read(handle, buf);
    
    // Print out the returned buffer.
    int i;
	for (i = 0; i < 4; i++)
		printf("buf[%d]: %d\n", i, buf[i]);
    
    res = finalize();
    return (int)buf[1];
}