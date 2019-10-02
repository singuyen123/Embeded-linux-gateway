#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <pthread.h>
#include "syslog.h"
#include "MQTTClient.h"

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
/* define for netlink*/
#define NETLINK_USER 31
#define NETLINK_USER1 21
#define MAX_PAYLOAD 1024 /* maximum payload size*/

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int rc;
int ch;

/* code netlink*/
void *netlink_rev(void *netlink_user) {
  struct sockaddr_nl src_addr, dest_addr;
  struct nlmsghdr *nlh = NULL;
  struct iovec iov;
  int sock_fd;
  struct msghdr msg;
  int nl = atoi(netlink_user);
  sock_fd = socket(PF_NETLINK, SOCK_RAW, nl);
  if (sock_fd < 0)
    return -1;

  memset(&src_addr, 0, sizeof(src_addr));
  src_addr.nl_family = AF_NETLINK;
  src_addr.nl_pid = getpid(); /* self pid */

  bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

  memset(&dest_addr, 0, sizeof(dest_addr));
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.nl_family = AF_NETLINK;
  dest_addr.nl_pid = 0;    /* For Linux Kernel */
  dest_addr.nl_groups = 0; /* unicast */

  nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
  memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
  nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
  nlh->nlmsg_pid = getpid();
  nlh->nlmsg_flags = 0;

  strcpy(NLMSG_DATA(nlh), "Hello");

  iov.iov_base = (void *)nlh;
  iov.iov_len = nlh->nlmsg_len;
  msg.msg_name = (void *)&dest_addr;
  msg.msg_namelen = sizeof(dest_addr);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  syslog(LOG_INFO, "Sending message to kernel\n");
  sendmsg(sock_fd, &msg, 0);
  syslog(LOG_INFO, "Waiting for message from kernel\n");

  /* Read message from kernel */
  while (1) {
    recvmsg(sock_fd, &msg, 0);
    printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
  }

  close(sock_fd);
}
/* end code netlink*/

/* code socket TCP*/
void func(int sockfd) {
  char buff[MAX];
  int n;
  // infinite loop for chat
  for (;;) {
    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    read(sockfd, buff, sizeof(buff));
    // print buffer which contains the client contents
    if (strncmp(buff, "usb", 3) == 0) {
      subscribe_topic("USB", "usb detected");
    }
    n = 0;
  }
}

void *run_socket_tcp() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    syslog(LOG_ERR, "socket creation failed...\n");
    exit(0);
  }
  else
    syslog(LOG_INFO, "Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT_TCP);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
    syslog(LOG_ERR, "socket bind failed...\n");
    exit(0);
  }
  else
    syslog(LOG_INFO, "Socket successfully binded..\n");

  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0) {
    syslog(LOG_ERR, "Listen failed...\n");
    exit(0);
  }
  else
    syslog(LOG_INFO, "Server listening..\n");
  len = sizeof(cli);

  // Accept the data packet from client and verification
  connfd = accept(sockfd, (SA *)&cli, &len);
  if (connfd < 0) {
    syslog(LOG_ERR, "server acccept failed...\n");
    exit(0);
  }
  else
    syslog(LOG_INFO, "server acccept the client...\n");

  // Function for chatting between client and server
  func(connfd);

  // After chatting close the socket
  close(sockfd);
}
/* end code socket TCP */

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
  syslog(LOG_INFO, "Message with token value %d delivery confirmed\n", dt);
  deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
  int i;
  char *payloadptr;

  syslog(LOG_INFO, "Message arrived\n");
  syslog(LOG_INFO, "     topic: %s\n", topicName);
  syslog(LOG_INFO, "   message: ");

  payloadptr = message->payload;
  for (i = 0; i < message->payloadlen; i++) {
    putchar(*payloadptr++);
  }
  putchar('\n');
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void connlost(void *context, char *cause) {
  syslog(LOG_INFO, "\nConnection lost\n");
  syslog(LOG_INFO, "     cause: %s\n", cause);
}

void subscribe_topic(char *topic, char *msg) {
  MQTTClient_subscribe(client, topic, QOS);

  pubmsg.payload = msg;
  pubmsg.payloadlen = (int)strlen(msg);
  pubmsg.qos = QOS;
  pubmsg.retained = 0;
  MQTTClient_publishMessage(client, topic, &pubmsg, &token);
  syslog(LOG_INFO, "Waiting for up to %d seconds for publication of %s\n"
                   "on topic %s for client with ClientID: %s\n",
         (int)(TIMEOUT / 1000), PAYLOAD, topic, CLIENTID);
  rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  syslog(LOG_INFO, "Message with delivery token %d delivered\n", token);

  do {
    ch = getchar();
  } while (ch != 'Q' && ch != 'q');

  MQTTClient_unsubscribe(client, topic);
}

int main(int argc, char *argv[]) {
  pthread_t thread_netlink1, thread_netlink2, thread_TCP;

  MQTTClient_create(&client, ADDRESS, CLIENTID,
                    MQTTCLIENT_PERSISTENCE_NONE, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;

  MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
    syslog(LOG_ERR, "Failed to connect, return code %d\n", rc);
    exit(EXIT_FAILURE);
  }
  syslog(LOG_INFO, "Subscribing to topic %s\nfor client %s using QoS%d\n\n"
                   "Press Q<Enter> to quit\n\n",
         TOPIC, CLIENTID, QOS);
  pthread_create(&thread_netlink1, NULL, netlink_rev, "31");
  pthread_create(&thread_netlink2, NULL, netlink_rev, "21");
  pthread_create(&thread_TCP, NULL, run_socket_tcp, NULL);

  pthread_join(thread_netlink1, NULL);
  pthread_join(thread_netlink2, NULL);
  run_socket_tcp();

  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);

  while (1)
    ;
  return rc;
}
