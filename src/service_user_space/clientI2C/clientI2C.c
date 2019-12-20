#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <poll.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include "src/i2cbusses.h"
#include "src/i2c.h"
#include "src/smbus.h"
#include "src/timer.h"
#include <json-c/json.h>

#define I2CBUS 0

/* define of socket TCP */
#define MAX 80
#define PORT 1000
#define SA struct sockaddr

int sockfd, connfd;
int var;
/*Variable I2c*/
static const char *device = "/dev/i2c-0";
char requestkey[5] = {'1', '9', '9', '7', '.'};
char requestData[3] = {'d', 'a', '*'};
static const char *msg_req_key = "send ACK to I2C";
static const char *msg_req_data = "send request data to I2C";
char recevie[50];
volatile int fd;
int a = 0;

const int checkKey = 97;
const int checkData = 99;

volatile int address = -1;
pthread_t id1;
/*thread function definition*/
void sendRequestToNode(const char b[], int length, const char *msg);
void *threadfunction2(void *args);
void timer_handler(void);
pthread_mutex_t mutexsum;

int scan_address(int file)
{
  printf("Start scan address\n");
  int i, res, flag = 0;
  for (i = 0; i < 128; i++)
  {
    if (var > 5)
    {
      var = 0;
      printf("Device don't Detected after scan\n");
      break;
    }
    else
    {
      if (ioctl(file, I2C_SLAVE, i) < 0)
      {
        // printf("Don't access\n");
      }
      if ((i >= 0x30 && i <= 0x37) || (i >= 0x50 && i <= 0x5F))
        res = i2c_smbus_read_byte(file);
      else
        res = i2c_smbus_write_quick(file, I2C_SMBUS_WRITE);
      if (res >= 0)
      {
        address = i;
        flag++;
      }
    }
  }

  if (flag != 0)
  {
    printf("address=%d\n", address);
    return address;
  }
  else
  {
    return 200;
  }
}
void socket_init()
{
  struct sockaddr_in servaddr, cli;

  // socket create and varification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    syslog(LOG_ERR, "socket creation failed...\n");
    // exit(0);
  }
  else
    syslog(LOG_INFO, "Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    syslog(LOG_INFO, "connection with the server failed...\n");
    // exit(0);
  }
  else
    syslog(LOG_INFO, "connected to the server..\n");
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
void *threadfunction2(void *args)
{
  while (1)
  {

    if (address > 128 || address < 0)
    {
      if (start_timer(500, &timer_handler))
      {
        printf("\n timer error\n");
        //continue;
      }
    }
    else
    {
      printf("address after scan\n");
      stop_timer();
    }

    memset(&recevie, '\0', sizeof(recevie));

    pthread_mutex_lock(&mutexsum);
    int num_bytes = read(fd, &recevie, sizeof(recevie));
    pthread_mutex_unlock(&mutexsum);

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0)
    {
      printf("Device don't Detected\n");
      //printf("Error reading1: %s\n", strerror(errno));
      pthread_mutex_lock(&mutexsum);
      a = 0;
      address = -1;
      start_timer(500, &timer_handler);
      //   close(fd);

      pthread_mutex_unlock(&mutexsum);
    }
    else
    {
      printf("Read %i bytes. Received message: %s\n", num_bytes, recevie);
      //num_bytes = 0;
      pthread_mutex_lock(&mutexsum);
      a = 1;
      pthread_mutex_unlock(&mutexsum);
    }

    while (a)
    {
      printf("nhay vao whilea\n");
      memset(&recevie, '\0', sizeof(recevie));

      pthread_mutex_lock(&mutexsum);
      int num_bytes1 = read(fd, &recevie, sizeof(recevie));
      pthread_mutex_unlock(&mutexsum);

      // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
      if (num_bytes1 < 0)
      {
        printf("Error reading2: %s", strerror(errno));

        pthread_mutex_lock(&mutexsum);
        a = 0;
        // close(fd);
        pthread_mutex_unlock(&mutexsum);
        // continue;
      }
      else
      {
        struct json_object *parsed_json;
        struct json_object *keynode;

        parsed_json = json_tokener_parse(recevie);
        json_object_object_get_ex(parsed_json, "keynode", &keynode);
        int checkResponse = json_object_get_int(keynode);
        printf("checkResponse: %d\n", checkResponse);
        if (checkResponse==checkKey)
        {
          //printf("Read %i bytes. Received message: %s\n", num_bytes, recevie);
          //   write(sockfd, "usb", sizeof("usb"));
          printf("Detected---------------------------------------------------------------------- \n");
        }
        if (checkResponse==checkData)
        {
          //printf("Read %i bytes. Received message: %s\n", num_bytes, recevie);
          printf("send data********************************************************************** \n");
        }
        //usleep(1000);
        sleep(1);
      }
    }

    sleep(2);
  }
}

/*handler timer*/
void timer_handler(void)
{
  var++;
  address = scan_address(fd);
  printf("address i2c=%02x\n", address);
  if (address < 128 && address > 0)
  {
    printf("-------------------------------Detected-------------------------------- \n");
    stop_timer();
  }
}

void sendRequestToNode(const char b[], int length, const char *msg)
{

  pthread_mutex_lock(&mutexsum);
  int ac1 = ioctl(fd, I2C_SLAVE, address);
  pthread_mutex_unlock(&mutexsum);
  if (ac1 < 0)
  {
    pthread_mutex_lock(&mutexsum);
    fd = open(device, O_RDWR);
    a = 0;
    pthread_mutex_unlock(&mutexsum);
    fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", address);
  }
  else
  {
    printf("%s\n", msg);
    pthread_mutex_lock(&mutexsum);
    int k1 = write(fd, b, length);
    pthread_mutex_unlock(&mutexsum);

    if (k1 < 0)
    {
      printf("Write fail\n");
    }
    fflush(stdout);
  }
}
int main(int argc, char *argv[])
{
  int rc;
  int ret;
  int count = 9;
  struct pollfd fds[1];
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;
  fd = open(device, O_RDWR);
  socket_init();

  printf("address first =%d\n", !address);
  if (start_timer(500, &timer_handler))
  {
    printf("\n timer error\n");
    //continue;
  }
  while (address < 0 || address > 128)
  {
    printf("---------");
  }

  /*creating thread*/
  ret = pthread_create(&id1, NULL, &threadfunction2, NULL);
  if (ret == 0)
  {
    printf("Thread created successfully.\n");
  }
  else
  {
    printf("Thread not created.\n");
    return 0; /*return from main*/
  }

  pthread_mutex_init(&mutexsum, NULL);

  while (1)
  {

    rc = poll(fds, 1, 1000); // 1s time out
    if (rc == 0)
    {

      printf("start detect I2C>>>>>>>>>>>>>>\n");
      count = (count + 1) % 10;
      if (count == 0)
      {
        sendRequestToNode(requestkey, sizeof(requestkey), msg_req_key);
      }
      else
      {
        sendRequestToNode(requestData, sizeof(requestData), msg_req_data);
      }
    }
  }

  return (EXIT_SUCCESS);
}