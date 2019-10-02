#ifndef SRC_COMMON_DATA_H_
#define SRC_COMMON_DATA_H_
#include <stdio.h>
#include <string.h>
#include <string>

enum Node {
    kUart,
    kI2c,
    kUSB
}

struct Data {
    Node node;
    string msg;
}

#endif  // COMMON_DATA_H_
