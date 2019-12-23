#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <RF24/LoRa.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../../../../common/data.h"

/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr

int sockfd, connfd;

struct Data data;
/**************************/
#define gpio_C 21
#define gpio_B 20
#define gpio_A 16

using namespace std;
int rc;
char nrfbuff[255];
char lorabuff[255];
LoRa_ctl modem;
// RPi generic:
RF24 radio(22,0);

/********************************/

// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t address[6] = "00001";

void rx_f(rxData *rx){
    data.node = kLora;
    strcpy(data.packet.lora.msg,rx->buf);
    rc = send(sockfd, &data, sizeof(data), 0);
	if(rc<=0)
	{
		printf("send error: %s\n",strerror(errno));
	}
	//printf("send data\n");
}

void socket_init()
{
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        // exit(0);
    }
    else
         printf( "Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
         printf( "connection with the server failed...\n");
         exit(0);
    }
    else
         printf( "connected to the server..\n");
}

int main(int argc, char** argv){

	int select=0;
	socket_init();
	
	if (gpioInitialise() < 0)
    {
        printf("Pigpio init error\n");
        return -1;
    }else{
		printf("inital gpio successfully\n");
	}
	gpioSetMode(gpio_A, PI_OUTPUT);
	gpioSetMode(gpio_B, PI_OUTPUT);
	gpioSetMode(gpio_C, PI_OUTPUT);
    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.rx.callback = rx_f;
    modem.rx.data.buf = lorabuff;
    modem.eth.preambleLen=6;
    modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
    modem.eth.sf = SF7;//Spreading Factor 12
    modem.eth.ecr = CR8;//Error coding rate CR4/8
    modem.eth.freq = 433000000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pi
    modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;//Output power
    modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
    modem.eth.AGC = 1;//Auto Gain Control
    modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;//Explicit header mode
    modem.eth.syncWord = 0x12;

	radio.begin();	
	radio.setRetries(15,15);

	radio.printDetails();
	radio.openReadingPipe(0, address);
	while(1)
	{
		switch (select) {
			case 0:
			 	radio.stopListening();
				gpioWrite(gpio_A, 0);
				gpioWrite(gpio_B, 0);
				gpioWrite(gpio_C, 0);
				LoRa_begin(&modem);
				LoRa_receive(&modem);
				sleep(2);
				break;
			case 1:
			{
				LoRa_end(&modem);
				gpioWrite(gpio_A, 1);
				gpioWrite(gpio_B, 1);
				gpioWrite(gpio_C, 1);
				radio.startListening();
				int count=0;
				while (count < 2){
				if ( radio.available() )
				{
					radio.read( &nrfbuff, sizeof(nrfbuff));
					data.node = kNRF;
					strcpy(data.packet.nrf.msg, nrfbuff);
					rc = send(sockfd, &data, sizeof(data), 0);
					if(rc<=0)
					{
						printf("send error: %s\n",strerror(errno));
					}
					count++;
					sleep(1);
				}
				}
				break;
			}
			case 2:
				radio.stopListening();
				gpioWrite(gpio_A, 1);
				gpioWrite(gpio_B, 0);
				gpioWrite(gpio_C, 0);
				LoRa_begin(&modem);
				LoRa_receive(&modem);
				sleep(2);
				break;
			case 3:

			{
				LoRa_end(&modem);
				gpioWrite(gpio_A, 0);
				gpioWrite(gpio_B, 1);
				gpioWrite(gpio_C, 1);
				radio.startListening();
				int count=0;
				while (count < 2){
				if ( radio.available() )
				{
					radio.read( &nrfbuff, sizeof(nrfbuff));
					data.node = kNRF;
					strcpy(data.packet.nrf.msg, nrfbuff);
					rc = send(sockfd, &data, sizeof(data), 0);
					if(rc<=0)
					{
						printf("send error: %s\n",strerror(errno));
					}
					count++;
					sleep(1);
				}
				}
				break;
			}
			case 4:
				radio.stopListening();
				gpioWrite(gpio_A, 0);
				gpioWrite(gpio_B, 1);
				gpioWrite(gpio_C, 0);
				LoRa_begin(&modem);
				LoRa_receive(&modem);
				sleep(2);
				break;
			case 5:

			{
				LoRa_end(&modem);
				gpioWrite(gpio_A, 1);
				gpioWrite(gpio_B, 0);
				gpioWrite(gpio_C, 1);
				radio.startListening();
				int count=0;
				while (count < 2){
				if ( radio.available() )
				{
					radio.read( &nrfbuff, sizeof(nrfbuff));
					data.node = kNRF;
					strcpy(data.packet.nrf.msg, nrfbuff);
					rc = send(sockfd, &data, sizeof(data), 0);
					if(rc<=0)
					{
						printf("send error: %s\n",strerror(errno));
					}
					count++;
					sleep(1);
				}
				}
				break;
			}

		}
		select = (select+1) % 6;
	}

  return 0;
}

