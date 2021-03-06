#ifndef SRC_COMMON_DATA_H_
#define SRC_COMMON_DATA_H_
#include <stdio.h>
enum Node {
    kUart,
    kI2c,
    kUSB,
    kWifi
};

struct DataI2c {
    char address[2];
    char msg[50];
};

struct DataUart {
    char msg[50];
};

struct DataUsb {
    char device_name[20];
    char msg[50];
};

struct DataWifi {
    char ip[15];
    char device_name[20];
    char msg[50];
};

union MSG {
  struct DataI2c i2c;
  struct DataUart uart;
  struct DataUsb usb;
  struct DataWifi wifi;
};

struct Data {
    enum Node node;
    union MSG msg;
};

#endif  // COMMON_DATA_H_
