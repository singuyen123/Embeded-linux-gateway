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
#include <poll.h>
#include "src/uart.h"
#include <json-c/json.h>
#include "../../common/data.h"
#include "src/wiringSerial1.h"
#include "src/timer.h"
#include "json-c/json.h"
#include "MQTTClient.h"

#define SA struct sockaddr
#define idBlue 97

/* define for mqtt_client */
#define ADDRESS "test.mosquitto.org"
#define CLIENTID "ExampleClientSub1"
#define TOPIC "presence"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

int sockfd, connfd;
static const char *device = "/dev/ttyS0";
volatile int fd;
struct Data data;
pthread_t id2;
char c;
char msg[50];

int rc;
int checkResponse =0;
struct json_object *parsed_json;
struct json_object *idNode;
/*thread function definition*/

void timer_handler(void);

void timer_handler(void)
{
  close(fd);
    if((fd = serialOpen1 ("/dev/ttyS0", 115200)) < 0 ){
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		}
    //stop_timer();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
  printf("Message with token value %d delivery confirmed\n", dt);
  deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
  int i;
  char *payloadptr;

  printf("Message arrived\n");
  printf("     topic: %s %d\n", topicName, sizeof(topicName));
  printf("   message: ");

  //payloadptr = message->payload;
  //for (i = 0; i < message->payloadlen; i++) {
  //  putchar(*payloadptr++);
  //}
  putchar('\n');
  if (!strncmp(topicName,"sdt_wifi",8)) {
    publish_mess("sdt_server","{\"type\":\"wifi\"}");
  }
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void connlost(void *context, char *cause) {
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
  //while (!rc) {
  //  rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  //}
  printf("Message with delivery token %d delivered\n", token);
}

int count=0;

int main(int argc, char *argv[])
{
  MQTTClient_create(&client, ADDRESS, CLIENTID,
                    MQTTCLIENT_PERSISTENCE_NONE, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;

  MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
    printf("Failed to connect, return code %d\n", rc);
    exit(EXIT_FAILURE);
  }
  if (start_timer(500, &timer_handler))
  {
    printf("\n timer error\n");
    //continue;
  }

  if((fd = serialOpen1 ("/dev/ttyS0", 115200)) < 0 ){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
  }
  while(1) {
    while(serialDataAvail1(fd)){
      int num_bytes1;
      memset(&msg, '\0', sizeof(msg));
      num_bytes1 = read(fd, msg, sizeof(msg));
      printf("num_bytes1 %d . Received message---- : %s\n", num_bytes1, msg);
      if (num_bytes1 < 0)
      {
        printf("Error reading2: %s", strerror(errno));
      }
      else if(num_bytes1>41){
        count++;
        parsed_json = json_tokener_parse(msg);
        json_object_object_get_ex(parsed_json, "id", &idNode);
        checkResponse = json_object_get_int(idNode);
        printf("checkResponse_Blue = %d\n",checkResponse);
        if(checkResponse == idBlue)
          publish_mess("data_bl",msg);
        printf("count %d . Received message---- : %s\n", count, msg);
      }
        fflush (stdout);
    }
	//	}
    usleep (50000) ;
  }
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
  return 0;
}
