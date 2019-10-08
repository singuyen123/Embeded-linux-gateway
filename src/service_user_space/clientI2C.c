#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "smbus.h"
#include <poll.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include "i2cbusses.h"
#include "i2c.h"

#define I2CBUS 0

/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr

int sockfd, connfd;
static const char *device = "/dev/i2c-0";
const char key[5] = {'1', '9', '9', '7', '.'};
const char requestData[3] = {'d', 'a', '*'};
char recevie[2];
volatile int fd;
int a = 0;
const char keyre[2] = {'9', '7'};
const char ask[2] = "OK";
int address = 9;
pthread_t id1;

/*thread function definition*/

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
    int num_bytes = read(fd, &recevie, sizeof(recevie));
    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0)
    {
      printf("Error reading1: %s\n", strerror(errno));
      pthread_mutex_lock(&mutexsum);
      a = 0;
      close(fd);
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
      num_bytes = read(fd, &recevie, sizeof(recevie));
      // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
      if (num_bytes < 0)
      {
        printf("Error reading2: %s", strerror(errno));

        pthread_mutex_lock(&mutexsum);
        a = 0;

        close(fd);
        pthread_mutex_unlock(&mutexsum);
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
      }
    //  usleep(5000);
    }
    //if (recevie[0] == '9' && recevie[1] == '7')
    sleep(2);
  }
}
int main(int argc, char *argv[])
{
  int rc;
  int ret;
  int count = 2;
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

      printf("start detect\n");
      count = (count + 1) % 3;
      if (count == 0)
      {
        if (ioctl(fd, I2C_SLAVE, address) < 0)
        {
          pthread_mutex_lock(&mutexsum);
          fd = open(device, O_RDWR);
          a = 0;
          pthread_mutex_unlock(&mutexsum);
          fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", address);
        }
        else
        {
          printf("before write1\n");
          int k = write(fd, &key, sizeof(key));
          if (k < 0)
          {
            printf("Write fail1\n");
          }
          fflush(stdout);
        }
      }
      else
      {
        if (ioctl(fd, I2C_SLAVE, address) < 0)
        {
          pthread_mutex_lock(&mutexsum);
          fd = open(device, O_RDWR);
          a = 0;
          pthread_mutex_unlock(&mutexsum);
          fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", address);
        }
        else
        {
          printf("before write2\n");
          int k = write(fd, &requestData, sizeof(requestData));
          if (k < 0)
          {
            printf("Write fail2\n");
          }
          fflush(stdout);
        }
      }
    }
  }

  // close(fd);
  return (EXIT_SUCCESS);
}