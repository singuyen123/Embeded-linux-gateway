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


#define I2CBUS 0

/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr

int sockfd, connfd;
static const char *device = "/dev/i2c-0";
 char key[5] = {'1', '9', '9', '7', '.'};
  char requestData[3] = {'d', 'a', '*'};
char recevie[2];
volatile int fd;
int a = 0;
const char keyre[2] = {'9', '7'};
const char ask[2] = "OK";
int address = 9;
pthread_t id1;

/*thread function definition*/
void sendRequestToNode(const char b[],int length);
void *threadfunction2(void *args);
void *threadFunction1(void *args);
pthread_mutex_t mutexsum;

int scan_address(int file)
{
  int i, res, flag = 0;
  for (i = 0; i < 128; i++)
  {
    if (ioctl(file, I2C_SLAVE, i) < 0)
    {
      printf("Don't access\n");
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

  if (flag != 0)
  {
    printf("address=%d\n", address);
    return address;
  }
  else
  {

    return 0;
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
      close(fd);
      pthread_mutex_unlock(&mutexsum);
      // continue;
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

        close(fd);
        pthread_mutex_unlock(&mutexsum);
        // continue;
      }
      else
      {

        if (recevie[0] == '9' && recevie[1] == '7')
        {
          //printf("Read %i bytes. Received message: %s\n", num_bytes, recevie);
          printf("Detected---------------------------------------------------------------------- \n");
        }
        if (recevie[0] == '9' && recevie[1] == '9')
        {
          //printf("Read %i bytes. Received message: %s\n", num_bytes, recevie);
          printf("send data********************************************************************** \n");
        }
        sleep(1);
      }
    }

    sleep(2);
  }
}

void sendRequestToNode(const char b[],int length)
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
    printf("Enter write\n");
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
  int count = 3;
  struct pollfd fds[1];
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;
  fd = open(device, O_RDWR);

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

      printf("start detect>>>>>>>>>>>>>>\n");
      count = (count + 1) % 4;
      if (count == 0)
      {
        sendRequestToNode(key,sizeof(key));
        // pthread_mutex_lock(&mutexsum);
        // int ac1 = ioctl(fd, I2C_SLAVE, address);
        // pthread_mutex_unlock(&mutexsum);
        // if (ac1 < 0)
        // {
        //   pthread_mutex_lock(&mutexsum);
        //   fd = open(device, O_RDWR);
        //   a = 0;
        //   pthread_mutex_unlock(&mutexsum);
        //   fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", address);
        // }
        // else
        // {
        //   printf("Enter write1\n");
        //   pthread_mutex_lock(&mutexsum);
        //   int k1 = write(fd, &key, sizeof(key));
        //   pthread_mutex_unlock(&mutexsum);

        //   if (k1 < 0)
        //   {
        //     printf("Write fail1\n");
        //   }
        //   fflush(stdout);
        // }
      }
      else
      {
        sendRequestToNode(requestData,sizeof(requestData));
        // pthread_mutex_lock(&mutexsum);
        // int ac2 = ioctl(fd, I2C_SLAVE, address);
        // pthread_mutex_unlock(&mutexsum);
        // if (ac2 < 0)
        // {
        //   pthread_mutex_lock(&mutexsum);
        //   fd = open(device, O_RDWR);
        //   a = 0;
        //   pthread_mutex_unlock(&mutexsum);
        //   fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", address);
        // }
        // else
        // {
        //   printf("Enter write2\n");

        //   pthread_mutex_lock(&mutexsum);
        //   int k2 = write(fd, &requestData, sizeof(requestData));
        //   pthread_mutex_unlock(&mutexsum);

        //   if (k2 < 0)
        //   {
        //     printf("Write fail2\n");
        //   }
        //   fflush(stdout);
        // }
      }
    }
  }

  // close(fd);
  return (EXIT_SUCCESS);
}