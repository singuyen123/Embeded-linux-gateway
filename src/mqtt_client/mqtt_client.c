#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <pthread.h>
#include "syslog.h"
#include "data.h"
#include "MQTTClient.h"
#include <errno.h>
#include <poll.h>

/* define for mqtt_client */
#define ADDRESS "test.mosquitto.org"
#define CLIENTID "ExampleClientSub"
#define TOPIC "presence"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L
/* define for socket TCP*/
#define MAX 80
#define PORT_TCP 6997
#define SA struct sockaddr

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int rc;
int ch;
struct Data data;

/* code socket TCP*/

void *run_socket_tcp()
{
  printf("run socket");
  int sockfd, connfd, len, nfds = 1, rc;
  struct sockaddr_in servaddr, cli;
  struct pollfd poll_fds[20];

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));
  poll_fds[0].events = POLLIN;
  poll_fds[0].fd = sockfd;
  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT_TCP);

  // Binding newly created socket to given IP and verification
  rc = (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)));
  printf("rc = %d", rc);
  if (rc != 0)
  {
    printf("socket bind failed...:%s\n", strerror(errno));
    exit(0);
  }
  else
    printf("Socket successfully binded..\n");

  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0)
  {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening..\n");
  while (1)
  {
    rc = poll(poll_fds, nfds, -1);
    if (rc < 0)
    {
      printf("poll fail");
    }
    if (rc == 0)
    {
      printf("time out \n");
    }
    for (int i = 1; i <= nfds; i++)
    {
      if (poll_fds[0].revents & POLLIN)
      {
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 0)
        {
          printf("server acccept failed...\n");
          // exit(0);
        }
        else
        {
          printf("server acccept the client...fd = %d\n", connfd);
          poll_fds[nfds].fd = connfd;
          poll_fds[nfds].events = POLLIN;
          nfds++;
          break;
        }
      }

      if (poll_fds[i].revents & POLLIN)
      {
        printf("event from client\n");
        rc = recv(poll_fds[i].fd, &data, sizeof(data), 0);
        if (rc <= 0)
        {
          printf("send error: %s\n",strerror(errno));
          printf("client disconected\n");
          close(poll_fds[i].fd);
          nfds--;
          break;
        }
        switch (data.node)
        {
        case kUart:
          printf("msg recive: from UART with data: %s\n", data.packet.uart.msg);
          break;
        case kLora:
          printf("msg recive: from lora with data: %s\n", data.packet.lora.msg);
          break;
	      case kNRF:
	        printf("msg recive: from nrf with data %s\n", data.packet.nrf.msg);
	        break;
        }
      }
    }
  }
  close(sockfd);
}
/* end code socket TCP */

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
  printf("Message with token value %d delivery confirmed\n", dt);
  deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
  int i;
  char *payloadptr;

  printf("Message arrived\n");
  printf("     topic: %s %d\n", topicName, sizeof(topicName));
  printf("   message: ");

  payloadptr = message->payload;
  for (i = 0; i < message->payloadlen; i++)
  {
    putchar(*payloadptr++);
  }
  putchar('\n');
  if (!strncmp(topicName,"sdt_wifi",8)) {
    publish_mess("sdt_server","{\"type\":\"wifi\"}");
  }
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
  while (!rc) {
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  }
  printf("Message with delivery token %d delivered\n", token);

}

int main(int argc, char *argv[])
{
  pthread_t thread_netlink1, thread_netlink2, thread_TCP;

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
  MQTTClient_subscribe(client, "sdt_server", QOS);
  MQTTClient_subscribe(client, "sdt_wifi",QOS);
  
  run_socket_tcp();
  while (1)
    ;

  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);

  return rc;
}
