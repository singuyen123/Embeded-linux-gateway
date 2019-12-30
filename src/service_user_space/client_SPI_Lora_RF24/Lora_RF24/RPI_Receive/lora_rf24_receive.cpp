#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
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

#include <RF24/RF24.h>
#include <RF24/LoRa.h>
#include <RF24/timer.h>
#include <json-c/json.h>
#include "../../../../common/data.h"

using namespace std;

/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr
#define idLora 95
#define idRF24 96

/**************************/
#define gpio_C 21
#define gpio_B 20
#define gpio_A 16
#define gpio_26 5

int sockfd, connfd;
struct Data data;

int checkResponse = 0;
struct json_object *parsed_json;
struct json_object *idNode;

struct json_object *parsed_json1;
struct json_object *idNode1;

int rc;
char nrfbuff[50];
char lorabuff[255];
volatile int countLora = 0;
volatile int count = 0;
LoRa_ctl modem;
// RPi generic:
RF24 radio(22, 0);

/********************************/

// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t address[6] = "00001";

void timer_handler(void)
{
    count = 6;
    //stop_timer();
}

void timer_handler_lora(void)
{
    countLora = 3;
}
void rx_f(rxData *rx)
{

    parsed_json = json_tokener_parse(rx->buf);
    json_object_object_get_ex(parsed_json, "id", &idNode);
    checkResponse = json_object_get_int(idNode);
    printf("checkResponseLora = %d\n", checkResponse);

    if (checkResponse == idLora)
    {
        countLora++;
        data.node = kLora;
        strcpy(data.packet.lora.msg, rx->buf);
        rc = send(sockfd, &data, sizeof(data), 0);
        if (rc <= 0)
        {
            printf("send error: %s\n", strerror(errno));
        }
    }
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
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
}

int main(int argc, char **argv)
{

    int select = 0;
    socket_init();

    if (gpioInitialise() < 0)
    {
        printf("Pigpio init error\n");
        return -1;
    }
    else
    {
        printf("inital gpio successfully\n");
    }
    gpioSetMode(gpio_A, PI_OUTPUT);
    gpioSetMode(gpio_B, PI_OUTPUT);
    gpioSetMode(gpio_C, PI_OUTPUT);
    gpioSetMode(gpio_26, PI_OUTPUT);
    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0; //Raspberry SPI CE pin number
    modem.rx.callback = rx_f;
    modem.rx.data.buf = lorabuff;
    modem.eth.preambleLen = 6;
    modem.eth.bw = BW62_5;            //Bandwidth 62.5KHz
    modem.eth.sf = SF7;               //Spreading Factor 12
    modem.eth.ecr = CR8;              //Error coding rate CR4/8
    modem.eth.freq = 433000000;       // 434.8MHz
    modem.eth.resetGpioN = 4;         //GPIO4 on lora RESET pi
    modem.eth.dio0GpioN = 17;         //GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;        //Output power
    modem.eth.powerOutPin = PA_BOOST; //Power Amplifire pin
    modem.eth.AGC = 1;                //Auto Gain Control
    modem.eth.OCP = 240;              // 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;     //Explicit header mode
    modem.eth.syncWord = 0x12;

    while (1)
    {
        switch (select)
        {
        case 0:
            printf("------------Lora------------- 00\n");
            // gpioWrite(gpio_A, 0);
            // gpioWrite(gpio_B, 0);
            // gpioWrite(gpio_C, 0);
            // gpioWrite(gpio_26, 0);

            // LoRa_begin(&modem);
            // LoRa_receive(&modem);

            // if (start_timer(2000, &timer_handler_lora))
            // {
            //     printf("\n timer error\n");
            // }

            // while (1)
            // {
            //     if (countLora > 2)
            //     {
            //         break;
            //     }
            // }
            // stop_timer();
            // countLora = 0;
            // LoRa_end(&modem);
            // gpioWrite(gpio_26, 1);
            break;

		/*RF24_01 CH07*/
        case 1:
        {
            printf("////////////////RF24_01////////////\n");
            gpioWrite(gpio_A, 1);
            gpioWrite(gpio_B, 1);
            gpioWrite(gpio_C, 1);
			gpioWrite(gpio_26, 1);

            radio.begin();
            radio.openReadingPipe(0, address);
            radio.startListening();

            if (start_timer(2000, &timer_handler))
            {
                printf("\n timer error\n");
            }
            printf("========================start receive=========================\n");
            while (count < 4)
            {
                if (radio.available())
                {
                    memset(nrfbuff, '\0', sizeof(nrfbuff));
                    radio.read(&nrfbuff, sizeof(nrfbuff));

                    parsed_json = json_tokener_parse(nrfbuff);
                    json_object_object_get_ex(parsed_json, "id", &idNode);
                    checkResponse = json_object_get_int(idNode);
                    //printf("checkResponse_RF24_00 = %d\n",checkResponse);
                    printf("Received _RF24_01 --- : %s\n", nrfbuff);
                    if (checkResponse == idRF24)
                    {
                        data.node = kNRF;
                        strcpy(data.packet.nrf.msg, nrfbuff);
                        rc = send(sockfd, &data, sizeof(data), 0);
                        if (rc <= 0)
                        {
                            printf("send error: %s\n", strerror(errno));
                        }
                        count++;
                    }
                }
            }
            stop_timer();
            count = 0;
            radio.stopListening();
            radio.closeReadingPipe(0);
            radio.powerDown();
            gpioWrite(gpio_26, 0);
            break;
        }
        case 2:
            printf("*****************************Lora 01************************\n");
            gpioWrite(gpio_A, 1);
            gpioWrite(gpio_B, 0);
            gpioWrite(gpio_C, 0);
            gpioWrite(gpio_26, 0);

            LoRa_begin(&modem);
            LoRa_receive(&modem);

            if (start_timer(2000, &timer_handler_lora))
            {
                printf("\n timer error\n");
            }

            while (1)
            {
                if (countLora > 2)
                {
                    break;
                }
            }
            stop_timer();
            countLora = 0;
            LoRa_end(&modem);
            gpioWrite(gpio_26, 1);
            break;

		/*RF24_02 CH03*/
        case 3:	
        {
            printf("------------------------RF24_02--------------------\n");
            gpioWrite(gpio_A, 1);
            gpioWrite(gpio_B, 1);
            gpioWrite(gpio_C, 0);

            radio.begin();
            radio.openReadingPipe(0, address);
            radio.startListening();

            if (start_timer(2000, &timer_handler))
            {
                printf("\n timer error\n");
            }
            printf("========================start receive=========================\n");
            while (count < 4)
            {
                if (radio.available())
                {
                    memset(nrfbuff, '\0', sizeof(nrfbuff));
                    radio.read(&nrfbuff, sizeof(nrfbuff));
                    parsed_json = json_tokener_parse(nrfbuff);
                    json_object_object_get_ex(parsed_json, "id", &idNode);
                    checkResponse = json_object_get_int(idNode);
                    //printf("checkResponse_RF24_02 = %d\n",checkResponse);
                    //	printf("Received _RF24_02 --- : %s\n", nrfbuff);
                    if (checkResponse == idRF24)
                    {
                        data.node = kNRF;
                        strcpy(data.packet.nrf.msg, nrfbuff);
                        rc = send(sockfd, &data, sizeof(data), 0);
                        if (rc <= 0)
                        {
                            printf("send error: %s\n", strerror(errno));
                        }
                        count++;
                    }
                }
            }
            stop_timer();
            count = 0;
            radio.stopListening();
            radio.closeReadingPipe(0);
            radio.powerDown();
            gpioWrite(gpio_26, 0);
            break;
        }
        case 4:
            printf("Lora 00\n");
            gpioWrite(gpio_A, 0);
            gpioWrite(gpio_B, 1);
            gpioWrite(gpio_C, 0);
            gpioWrite(gpio_26, 0);

            LoRa_begin(&modem);
            LoRa_receive(&modem);

            if (start_timer(2000, &timer_handler_lora))
            {
                printf("\n timer error\n");
            }

            while (1)
            {
                if (countLora > 2)
                {
                    break;
                }
            }
            stop_timer();
            countLora = 0;
            LoRa_end(&modem);
            gpioWrite(gpio_26, 1);
            break;
		/*RF24_00 CH05*/
        case 5:
        {
            printf("RF24_00\n");
            gpioWrite(gpio_A, 1);
            gpioWrite(gpio_B, 0);
            gpioWrite(gpio_C, 1);

            radio.begin();
            radio.openReadingPipe(0, address);
            radio.startListening();

            if (start_timer(2000, &timer_handler))
            {
                printf("\n timer error\n");
            }
            printf("========================start receive=========================\n");
            while (count < 4)
            {
                if (radio.available())
                {
                    memset(nrfbuff, '\0', sizeof(nrfbuff));
                    radio.read(&nrfbuff, sizeof(nrfbuff));
                    parsed_json = json_tokener_parse(nrfbuff);
                    json_object_object_get_ex(parsed_json, "id", &idNode);
                    checkResponse = json_object_get_int(idNode);
                    //printf("checkResponse_RF24_00 = %d\n",checkResponse);
                    	printf("Received _RF24_00 --- : %s\n", nrfbuff);
                    if (checkResponse == idRF24)
                    {
                        data.node = kNRF;
                        strcpy(data.packet.nrf.msg, nrfbuff);
                        rc = send(sockfd, &data, sizeof(data), 0);
                        if (rc <= 0)
                        {
                            printf("send error: %s\n", strerror(errno));
                        }
                        count++;
                    }
                }
            }
            stop_timer();
            count = 0;
            radio.stopListening();
            radio.closeReadingPipe(0);
            radio.powerDown();
            gpioWrite(gpio_26, 0);
            break;
        }
        }
        select = (select + 1) % 6;
    }
    return 0;
}
