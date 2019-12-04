#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

#include "wiringPi.h"
#include "wiringPiSPI.h"
using namespace std;
#define chanel 0
#define speed 800000
#define SS0 10 // GPIOC3 10
#define CE 1	//GPIOA6


const uint64_t pipes[2] = {0xABCDABCD71LL, 0x544d52687CLL}; // Radio pipe addresses for the 2 nodes to communicate.
char data[32] = {"_A message from RPi w/ NRF24L+!"};        //Data buffer
volatile int fd;
void showData(void);
void setUpSPI();

 

int main(int argc, char *argv[])
{
    /***********************************/
	setUpSPI();
    // Setup and configure rf radio
    RF24 radio(CE,SS0,speed);
    radio.begin();

    // optionally, increase the delay between retries & # of retries
    radio.setRetries(15, 15);
    // Set the channel
    radio.setChannel(0);
    // Set the data rate
    //radio.setDataRate(RF24_2MBPS);
    radio.setDataRate(RF24_250KBPS);
    //radio.setPALevel(RF24_PA_MAX);
    radio.setPALevel(RF24_PA_MIN);

    radio.openReadingPipe(0, pipes[1]);
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
        delayRF24(5);
    } // forever loop

    return 0;
}

void setUpSPI()
{
    printf("start SPI\n");
    pinModeWiringPi(SS0, OUTPUT);
    digitalWriteWiringPi(SS0, 1);
     pinModeWiringPi(CE, OUTPUT);
    digitalWriteWiringPi(CE, 0)
    if ((wiringPiSPISetup(chanel, speed)) < 0)
    {
        fprintf(stderr, "SPI Setup failed: %s\n", strerror(errno));
    }

    fd = wiringPiSPIGetFd(0);
}
void showData(void)  
{  
      printf("Data: ");  
      for(int i=0; i<32; i++){  
         if(isprint(data[i]))  
           printf("%c", data[i]);  
         else  
           printf(".");  
      }  
      printf("\n\r");  
} 