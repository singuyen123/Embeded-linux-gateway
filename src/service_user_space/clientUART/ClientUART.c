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
//#include "wiringPi.h"
/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr

int flagDetect = 0;
int sockfd, connfd;
static const char *device = "/dev/ttyS0";

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



int main(int argc, char *argv[])
{
    struct pollfd fds[2];
    socket_init();
    /*
    fd = serialOpen("/dev/ttyS0", 9600);
    int num_bytes1;
     while(1)
     {
       
        printf("access read \n");
        // memset(&msg, '\0', sizeof(msg));
        num_bytes1 = read(fd, msg, sizeof(msg));

        printf("Read %i bytes. Received message---- : %s\n", num_bytes1, msg);
        // memcpy(data.msg.uart.msg,msg, sizeof(msg));
        // data.node = kUart;
        // send(sockfd, &data, sizeof(data),0);
       
        if (num_bytes1 < 0)
        {
            printf("Error reading2: %s", strerror(errno));
        }
     }
    return (EXIT_SUCCESS);
    */
     
	int fd,rc;
	char c;
	printf("Raspberry's receiving : \n");
   fd = serialOpen1 ("/dev/ttyS0", 115200);
        fds[0].events = POLLIN;
        fds[0].fd = fd;
	while(1) {
  /*     fd = serialOpen1 ("/dev/ttyS0", 9600);
			while(serialDataAvail1(fd)){
      fd = serialOpen1 ("/dev/ttyS0", 9600);
      */
        fd = serialOpen1 ("/dev/ttyS0", 115200);
        fds[0].events = POLLIN;
        fds[0].fd = fd;
        rc = poll(fds, 1, -1);
        if (rc<=0) {
          printf("poll fail");
        }
        if (fds[0].revents & POLLIN){
        int num_bytes1;
        memset(&msg, '\0', sizeof(msg));
        // for (int i=0; i<10000; i++);
        num_bytes1 = read(fds[0].fd, msg, sizeof(msg));
       
        if (num_bytes1 < 0)
        {
            printf("Error reading2: %s", strerror(errno));
            
        }
        else{
          printf("Read %i bytes. Received message---- : %s\n", num_bytes1, msg);
        }
        }
       /*
				c = serialGetchar1(fd);
				printf("%c",c);
				fflush (stdout);
			}
		}*/
   }
	return 0;
}
