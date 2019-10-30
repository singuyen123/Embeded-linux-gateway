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
#include <json-c/json.h>
#include "wiringPi.h"
#include "wiringPiSPI.h"
/* define of socket TCP */
#define MAX 80
#define PORT 1000
#define SA struct sockaddr
/*define parameter SPI*/
#define chanel 0
#define speed 500000
#define SS0 10 // GPIOG3 10
int sockfd, connfd;

static const char *API_key = "----------send ACK to SPI---------";
static const char *msg_send_data = "********send data to SPI************";
char requestKey[2] = {'b', '.'};
char requestData[2] = {'a', '.'};
char msgReceive[50];
char msgReceive1[50];
volatile int ka = 0;

volatile int fd;
int flagDetect = 0;

const int responeKey = 99;
const int responeData = 97;
volatile int flag_no_receive = 1;
pthread_t id1;

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
int flag1 = 0;
int flag2 = 0;
void *threadfunction1(void *args)
{
    while (flag_no_receive)
    {
        printf("-----In Thread 1----\n");
        // memset(&msgReceive, '\0', sizeof(msgReceive));
        //     int num_byte= wiringPiSPIDataRW(chanel,msgReceive,50);
        int num_byte = read(fd, msgReceive, sizeof(msgReceive));
        printf("Read %i bytes. Received message------ : %s\n", num_byte, msgReceive);
        usleep(200000);
        //  printf("-----------Received message11111111--- : %c\n", msgReceive[0]);
        // printf("-----------Received messa22222222--- : %c\n", msgReceive[1]);
        if (msgReceive[0] == '{')
        {
            flag1 = 1;
        }
        if (msgReceive[0] == '.')
        {
            flag1 = 0;
            memset(&msgReceive1, '\0', sizeof(msgReceive1));
        }
        if (msgReceive[0] == '}' && msgReceive1[0] == '{')
        {
            flag1 = 0;
            msgReceive1[ka] = msgReceive[0];
            printf("Received message11111111--- : %s\n", msgReceive1);
            ka = 0;
            memset(&msgReceive1, '\0', sizeof(msgReceive1));
        }
        if (flag1 == 1 && msgReceive[0] != '.' && msgReceive[0] != 'a' && msgReceive[0] != 'b')
        {
            msgReceive1[ka] = msgReceive[0];
            //    memset(&msgReceive, '\0', sizeof(msgReceive));
            ka++;
            // if (ka == 6)
            // {
            //     printf("Received message11111111--- : %s\n", msgReceive1);
            //     ka = 0;
            // }
        }

        //    sleep(1);
        //  if(msgReceive[4]=='}')
        //{
        //  printf("array 4 %s",msgReceive[4]);
        //  memset(&msgReceive, '\0', sizeof(msgReceive));
        //}

        // pthread_mutex_lock(&mutexsum);
        // int num_bytes1 = read(fd, &msgReceive, sizeof(msgReceive));
        // pthread_mutex_unlock(&mutexsum);
        // printf("Read %i bytes. Received message------ : %s\n", num_bytes1, msgReceive);
        // // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        // if (num_bytes1 < 0)
        // {
        //     printf("Error reading2: %s", strerror(errno));
        // }
        // else
        // {
        //     // printf("start read\n");
        //     // struct json_object *parsed_json;
        //     // struct json_object *keynode;

        //     // parsed_json = json_tokener_parse(msgReceive);
        //     // json_object_object_get_ex(parsed_json, "keynode", &keynode);

        //     // int checkResponse = json_object_get_int(keynode);
        //     // printf("checkResponse: %d\n", checkResponse);

        //     // printf("Read %i bytes. Received message***** : %s\n", num_bytes1, msgReceive);
        //     // if (checkResponse == responeKey)
        //     // {
        //     //     printf("Read %i bytes. Received message : %s\n", num_bytes1, msgReceive);
        //     //     printf("Detected---------------------------------------------------------------------- \n");
        //     //     flagDetect = 0;
        //     // }
        //     // else if (checkResponse == responeData)
        //     // {
        //     //     printf("Read %i bytes. Received message : %s\n", num_bytes1, msgReceive);
        //     //     printf("send data********************************************************************** \n");
        //     //     flagDetect = 0;
        //     // }
        //     // else
        //     // {
        //     //     flagDetect++;
        //     //     if (flagDetect > 10)
        //     //     {
        //     //         printf("No connect device\n");
        //     //         flagDetect = 0;
        //     //     }
        //     // }
        // }

        // int num_bytes1= wiringPiSPIDataRW(chanel,&msg_send_data,10);
        //         printf("Read %i bytes. Received message------ : %s\n", num_bytes1, msgReceive);
        // usleep(5000);
        //    sleep(1);
        fflush(stdout);
    }
}
void setUpSPI()
{
    pinMode(SS0, OUTPUT);
    digitalWrite(SS0, 1);
    if ((wiringPiSPISetup(chanel, speed)) < 0)
    {
        fprintf(stderr, "SPI Setup failed: %s\n", strerror(errno));
        // return 0;
    }

    fd = wiringPiSPIGetFd(0);
}
void sendRequestToNode(const char b[], int length, const char *msg)
{
    pthread_mutex_lock(&mutexsum);
    flag_no_receive = 0;
    pthread_mutex_unlock(&mutexsum);
    printf("%s\n", msg);
    digitalWrite(SS0, 0);

    pthread_mutex_lock(&mutexsum);
    int k1 = write(fd, b, length);
    pthread_mutex_unlock(&mutexsum);

    digitalWrite(SS0, 1);

    if (k1 < 0)
    {
        printf("Write to uart fail\n");
    }
    pthread_mutex_lock(&mutexsum);
    flag_no_receive = 1;
    pthread_mutex_unlock(&mutexsum);
    fflush(stdout);
}
int main(int argc, char *argv[])
{
    int rc;
    int ret;
    int count = 4;
    struct pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    setUpSPI();
    // /*creating thread*/
    // ret = pthread_create(&id1, NULL, &threadfunction1, NULL);
    // if (ret == 0)
    // {
    //     printf("Thread created successfully.\n");
    // }
    // else
    // {
    //     printf("Thread not created.\n");
    //     return 0; /*return from main*/
    // }

    // pthread_mutex_init(&mutexsum, NULL);

    while (1)
    {

        rc = poll(fds, 1, 5000); // 1s time out
        if (rc == 0)
        {
            //  printf("start detect SPI>>>>>>>>>>>>>>\n");
            count = (count + 1) % 5;
            if (count == 0)
            {
                pthread_cancel(id1);

                sendRequestToNode(requestKey, sizeof(requestKey), API_key);

                ret = pthread_create(&id1, NULL, &threadfunction1, NULL);
                if (ret == 0)
                {
                    printf("Thread created again successfully.\n");
                }
                else
                {
                    printf("Thread not created again.\n");
                    return 0; /*return from main*/
                }

                pthread_mutex_init(&mutexsum, NULL);
            }
            else
            {
                pthread_cancel(id1);
                sendRequestToNode(requestData, sizeof(requestData), msg_send_data);
                ret = pthread_create(&id1, NULL, &threadfunction1, NULL);
                if (ret == 0)
                {
                    printf("Thread created again successfully.\n");
                }
                else
                {
                    printf("Thread not created again.\n");
                    return 0; /*return from main*/
                }

                pthread_mutex_init(&mutexsum, NULL);
            }
        }
    }
    return (EXIT_SUCCESS);
}
