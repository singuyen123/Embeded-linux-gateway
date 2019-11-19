#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
//#include <json-c/json.h>
#include "../../common/data.h"
#include "../../library/RF24/RF24.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr
/*define parameter SPI*/
#define chanel 0
#define speed 500000
#define SS0 10 // GPIOG3 10

RF24 radio(CE, SS0, speed);
const uint64_t pipes[2] = {0xABCDABCD71LL, 0x544d52687CLL}; // Radio pipe addresses for the 2 nodes to communicate.
char data[32] = {"_A message from RPi w/ NRF24L+!"};        //Data buffer

int main(int argc, char *argv[])
{
    /***********************************/
    // Setup and configure rf radio
    radio.begin();

    // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);
    // Set the channel
    radio.setChannel(1);
    // Set the data rate
    //radio.setDataRate(RF24_2MBPS);
    radio.setDataRate(RF24_250KBPS);
    //radio.setPALevel(RF24_PA_MAX);
    radio.setPALevel(RF24_PA_MIN);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
    memset(&data, '\0', sizeof(data));
    radio.startListening();

    radio.printDetails();
    printf("Start loop:\n");
    // forever loop
    while (1)
    {
        if (radio.available())
        {
            // Read any available payloads for analysis
            radio.read(&data, 32);
            // Dump the printable data of the payload
            showData();
            fflush(stdout);
        }
        // }
        delay(5);
    } // forever loop

    return 0;
}
