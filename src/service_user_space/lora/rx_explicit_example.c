#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <syslog.h>
#include <poll.h>
#include "data.h"
#include "LoRa.h"

#define MAX 80
#define PORT 6997
#define SA struct sockaddr
struct Data data;
LoRa_ctl modem;
char rxbuf[255];
void rx_f(rxData *rx){
    printf("rx done \n");
    //printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    printf("string: %s\n", rx->buf);//Data we'v received
    printf("RSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
}

void lora_init() {
    modem.spiCS = 1;//Raspberry SPI CE pin number
    modem.rx.callback = rx_f;
    modem.rx.data.buf = rxbuf;
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
    //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf
}

void delay(int number_of_seconds) {
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Stroing start time
  clock_t start_time = clock();

  // looping till required time is not acheived
  while (clock() < start_time + milli_seconds)
    ;
}

int main() {
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  lora_init();
  LoRa_begin(&modem);
  LoRa_receive(&modem);
  // socket create and varification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    syslog(LOG_ERR, "socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
    printf( "connection with the server failed...\n");
    exit(0);
  }
  else
    printf( "connected to the server..\n");
  while(1) {
    printf("send data\n");
    data.node = kI2c;
    send(sockfd, &data, sizeof(data),0);
    delay(1000);
  }
  close(sockfd);
}


