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
#include <json-c/json.h>
#include "../../common/data.h"

/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr

int flagDetect = 0;
int sockfd, connfd;
static const char *device = "/dev/ttyS0";
static const char *API_key = "send ACK to uart2";
static const char *request_data = "send data to uart2";
volatile int fd;
struct Data data;
pthread_t id2;

char msg[50];
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
        printf("socket creation failed...\n");
        // exit(0);
    }
    else
         printf( "Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
         printf( "connection with the server failed...\n");
        // exit(0);
    }
    else
         printf( "connected to the server..\n");
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
void *threadfunction1(void *args)
{
    while (1)
    {
        printf("-----In Thread 1----\n");

        printf("access read \n");
        memset(&msg, '\0', sizeof(msg));

        pthread_mutex_lock(&mutexsum);
        int num_bytes1 = read(fd, &msg, sizeof(msg));
        pthread_mutex_unlock(&mutexsum);

        printf("num_byte1: %d\n", num_bytes1);
        printf("Read %i bytes. Received message---- : %s\n", num_bytes1, msg);
        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes1 < 0)
        {
            printf("Error reading2: %s", strerror(errno));
        }
        else
        {
            printf("start read\n");

        }
    }
}


int main(int argc, char *argv[])
{
    int rc;
    int ret;
    int count = 9;
    socket_init();
    struct pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    fd = serialOpen("/dev/ttyS0", 9600);
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
    return (EXIT_SUCCESS);
}
