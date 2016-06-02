#include "usxfwdll.h"

#ifndef MAX_STR
#define MAX_STR 255
#endif
#ifndef DELAY_MS
#define DELAY_MS 2
#endif

unsigned short VID = 0x1278;
unsigned short PID = 0x0920;

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

int finalise()
{
    // finalize hidapi library
    return hid_exit();
}

hid_device* open_device()
{
    // open FW device and get handle for read/write
    return hid_open(VID, PID, NULL);
}

void close_device(hid_device* handle){
    hid_close(handle);
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

int request_filter(hid_device* handle, int filter_num)
{
    /* Send a command to set the current filter.
    
    Returns immediately, does not block
    */
    int res;
    unsigned char buf[2];
    buf[0] = filter_num;
    buf[1] = 0x0;
    res = hid_write(handle, buf, 2);
    if (res != 2){
       return res;
    }
    res = wait_and_read(handle, buf);
    return res;
}

int check_curr_filter(hid_device* handle)
{
    /* Request the current filter number from the filter wheel.a
    
    May be 0, which indicates filter is still moving.
    */
    int res;
    unsigned char buf[2];

    buf[0] = 0x0;
    buf[1] = 0x0;
    res = hid_write(handle, buf, 2);
    wait_and_read(handle, buf);
    return (int)buf[0];
}

