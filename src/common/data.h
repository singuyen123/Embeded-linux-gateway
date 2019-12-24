#ifndef SRC_COMMON_DATA_H_
#define SRC_COMMON_DATA_H_
#include <stdio.h>
enum Node {
    kUart,
    kLora,
    kNRF,
    kSPI,
    kWifi
};

struct DataNRF {
    char address[2];
    char msg[255];
};

struct DataUart {
    char msg[255];
    double dmsg;
};

struct DataLora {
    char device_name[20];
    char msg[255];
};

struct DataWifi {
    char ip[15];
    char device_name[20];
    char msg[255];
};

union MSG {
	struct DataNRF nrf;
	struct DataUart uart;
	struct DataLora lora;
	struct DataWifi wifi;
};

struct Data {
    enum Node node;
    union MSG packet;
};


#endif  // COMMON_DATA_H_