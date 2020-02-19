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
#include "MQTTClient.h"
#include "../../../../common/data.h"

using namespace std;

/* define for mqtt_client */
#define ADDRESS "test.mosquitto.org"
#define CLIENTID "ExampleClientSub"
#define TOPIC "presence"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

#define idLora 95
#define idRF24 96

/**************************/
#define gpio_C 21
#define gpio_B 20
#define gpio_A 16
#define gpio_26 5

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int ch;

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
//MQTT 
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
  printf("Message with token value %d delivery confirmed\n", dt);
  deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
 // int i;
  //char *payloadptr;

  printf("Message arrived\n");
  printf("     topic: %s %d\n", topicName, sizeof(topicName));
  printf("   message: ");

  // payloadptr = message->payload;
  // for (i = 0; i < message->payloadlen; i++)
  // {
    // putchar(*payloadptr++);
  // }
  putchar('\n');
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void connlost(void *context, char *cause)
{
  printf("\nConnection lost\n");
  printf("     cause: %s\n", cause);
}

void publish_mess(char *topic, char *msg) {
  pubmsg.payload = msg;
  pubmsg.payloadlen = (int)strlen(msg);
  pubmsg.qos = QOS;
  pubmsg.retained = 0;
  MQTTClient_publishMessage(client, topic, &pubmsg, &token);
  printf("Waiting for up to %d seconds for publication of %s\n"
         "on topic %s for client with ClientID: %s\n",
         (int)(TIMEOUT / 1000), PAYLOAD, topic, CLIENTID);
  // while (!rc) {
    // rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  // }
  printf("Message with delivery token %d delivered\n", token);

}

//end MQTT
// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t address[6] = "00001";

void timer_handler(void)
{
    count = 6;
    //stop_timer();
}

void timer_handler_lora(void)
{
    printf("----------------------------------timeoutLora------------------------\n");
    countLora = 10;
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
        publish_mess("data_lora",rx->buf);
  
    }


}

void timer_reset_all_lora(void)
{
    printf("*****!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////////////////***************Reset Lora******************!!!!!!!!!****///////////////*****");
    gpioSetMode(gpio_A, PI_OUTPUT);
    gpioSetMode(gpio_B, PI_OUTPUT);
    gpioSetMode(gpio_C, PI_OUTPUT);
    gpioSetMode(gpio_26, PI_OUTPUT);

    memset(lorabuff, '\0', sizeof(lorabuff));
   // memset(rx_f, '\0', sizeof(rx_f));

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
}

int main(int argc, char **argv)
{


    int select = 0;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
                    MQTTCLIENT_PERSISTENCE_NONE, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;

  MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
  {
    printf("Failed to connect, return code %d\n", rc);
    exit(EXIT_FAILURE);
  }

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

    // printf("start timer reset lora\n");
    // if (start_timer(125000, &timer_reset_all_lora))
    // {
    //      printf("\n timer error\n");
    // }

    while (1)
    {
        switch (select)
        {
        case 0:
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
                    //printf("Received _RF24_01 --- : %s\n", nrfbuff);
                    if (checkResponse == idRF24)
                    {
                        printf("%s\n",nrfbuff);
			            publish_mess("data_rf",nrfbuff);
		               count++;
                    }
                }
            }
            stop_timer();
            count = 0;
            radio.stopListening();
            radio.closeReadingPipe(0);
            radio.powerDown();
          //  gpioWrite(gpio_26, 0);
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
            printf("Count lora : %d ----------------\n",countLora);
            countLora = 0;

            LoRa_end(&modem);
            break;

		/*RF24_02 CH03*/
        case 3:	
        {
            printf("------------------------RF24_02--------------------\n");
            gpioWrite(gpio_A, 1);
            gpioWrite(gpio_B, 1);
            gpioWrite(gpio_C, 0);
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
                    if (checkResponse == idRF24)
                    {
                        publish_mess("data_rf",nrfbuff);
		                 count++;
                    }
                }
            }
            stop_timer();
            count = 0;
            radio.stopListening();
            radio.closeReadingPipe(0);
            radio.powerDown();
           // gpioWrite(gpio_26, 0);
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
          //  gpioWrite(gpio_26, 1);
            break;
		/*RF24_00 CH05*/
        case 5:
        {
            printf("RF24_00\n");
            gpioWrite(gpio_A, 1);
            gpioWrite(gpio_B, 0);
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
                    //printf("Received _RF24_00 --- : %s\n", nrfbuff);
                    if (checkResponse == idRF24)
                    {
			            printf("%s\n",nrfbuff);
			            publish_mess("data_rf",nrfbuff);
                        count++;
                    }
                }
            }
            stop_timer();
            count = 0;
            radio.stopListening();
            radio.closeReadingPipe(0);
            radio.powerDown();
           // gpioWrite(gpio_26, 0);
            break;
        }
        }
        select = (select + 1) % 6;
    }
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}
