#include<stdio.h>
#include<errno.h>

#include "wiringPi.h"
#include "wiringPiI2C.h"

#define ADDRESS 0x08;
const int key[4]={1,9,9,6};
static const char *devName = "/dev/i2c-0";

int main(){
    int fd;
    int readBytes;
    fd = wiringPiI2CSetupInterface(devName,ADDRESS);
    fprintf(stderr,"I2C: Failed to access %d\n",devName);
    
    while(1){
        wiringPiI2CWrite(fd,key);
          sleep(10);
    }
    
    
}