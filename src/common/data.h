#ifndef SRC_COMMON_DATA_H_
#define SRC_COMMON_DATA_H_
#include <stdio.h>
#include <string.h>
#include <string>

enum Node {
    kUart,
    kI2c,
    kUSB,
    kWifi
}

struct DataI2c {
    char address[2];
    char msg[50];
}

struct DataUart {
    char msg[50];
}

struct DataUsb {
    char device_name[20];
    char msg[50];
}

struct DataWifi {
    char ip[15];
    char device_name[20];
    char msg[50];
}

struct Data {
    Node node;
    union msg {
	DataI2c i2c;
	DataUart uart;
	DataUsb usb;
	DataWifi wifi;
    }
}

#endif  // COMMON_DATA_H_
