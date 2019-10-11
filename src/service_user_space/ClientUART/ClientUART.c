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
#include "src/uart.h"

/* define of socket TCP */
#define MAX 80
#define PORT 1000
#define SA struct sockaddr

int sockfd, connfd;
static const char *device = "/dev/ttyS2";
static const char *msg_send_key = "send ACK to uart2";
static const char *msg_send_data = "send data to uart2";
char key[5] = {'1', '9', '9', '7', '.'};
char requestData[3] = {'d', 'a', '*'};
char recevie[2];
volatile int fd;
int a = 0;
const char responeKey[2] = {'9', '7'};
const char responeData[2] = {'9', '9'};

pthread_t id2;

/*thread function definition*/

void sendRequestToNode(const char b[], int length, const char *msg);
void *threadFunction1(void *args);
pthread_mutex_t mutexsum;

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
void *threadfunction1(void *args)
{
    while (1)
    {
        printf("-----In Thread 1----\n");

        printf("access read \n");
        memset(&recevie, '\0', sizeof(recevie));

        pthread_mutex_lock(&mutexsum);
        int num_bytes1 = read(fd, &recevie, sizeof(recevie));
        pthread_mutex_unlock(&mutexsum);
        printf("num_byte1: %d\n", num_bytes1);
        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes1 < 0)
        {
            printf("Error reading2: %s", strerror(errno));

            pthread_mutex_lock(&mutexsum);
            //  a = 0;
            close(fd);
            pthread_mutex_unlock(&mutexsum);
            // continue;
        }
        else
        {
            printf("start read\n");
            if (recevie[0] == '9')
            {
                printf("Read %i bytes. Received message in while a: %s\n", num_bytes1, recevie);
                printf("Detected---------------------------------------------------------------------- \n");
            }
            else if (recevie[0] == '7')
            {
                printf("Read %i bytes. Received message in while a: %s\n", num_bytes1, recevie);
                printf("send data********************************************************************** \n");
            }
            else{
                printf("No connect device\n");
            }
        }


        sleep(2);
    }
}

void sendRequestToNode(const char b[], int length, const char *msg)
{
    printf("%s\n", msg);

    pthread_mutex_lock(&mutexsum);

    int k1 = write(fd, b, length);
    pthread_mutex_unlock(&mutexsum);

    if (k1 < 0)
    {
        printf("Write to uart fail\n");
    }
    fflush(stdout);
 
}
int main(int argc, char *argv[])
{
    int rc;
    int ret;
    int count = 3;
    struct pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    fd = serialOpen("/dev/ttyS2", 9600);
    /*creating thread*/
    ret = pthread_create(&id2, NULL, &threadfunction1, NULL);
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

        rc = poll(fds, 1, 2000); // 1s time out
        if (rc == 0)
        {
            pthread_mutex_lock(&mutexsum);
            fd = serialOpen("/dev/ttyS2", 9600);
            pthread_mutex_unlock(&mutexsum);
            printf("start detect uart>>>>>>>>>>>>>>\n");
            count = (count + 1) % 4;
            if (count == 0)
            {
                sendRequestToNode(key, sizeof(key), msg_send_key);
            }
            else
            {
                sendRequestToNode(requestData, sizeof(requestData), msg_send_data);
            }
        }
    }
    return (EXIT_SUCCESS);
}
