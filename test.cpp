#include "usxfwdll.h"

int main(int argc, char *argv[]){ 
    int filt_num = atoi( argv[1] );
    // initialise library
    initialise();

    // get handle for FW
    hid_device* fw;
    fw = open_device();
    
    // request new filter position
    int res = request_filter(fw, filt_num);
    
    // check filter position
    int pos;
    for (int i=0; i<100; i++){
        pos = check_curr_filter(fw);
        if (pos == filt_num){
            break;
        }
        sleep(100);
    }
    printf("Position now: %d\n", pos);
    close_device(fw);
    finalise();
}