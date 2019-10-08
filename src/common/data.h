#ifndef SRC_COMMON_DATA_H_
#define SRC_COMMON_DATA_H_
#include <stdio.h>
#include <string.h>

enum Node {
    kUart,
    kI2c,
    kUSB
};


struct Data {
  enum  Node node;
    union msg
    {
        char msgUart[10];
        char msgI2c[20];
        char msgUSB[20];
    };
    
}

#endif  // COMMON_DATA_H_
