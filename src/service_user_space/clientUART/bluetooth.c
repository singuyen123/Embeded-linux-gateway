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
//#include "wiringPi.h"
/* define of socket TCP */
#define MAX 80
#define PORT 6997
#define SA struct sockaddr

int sockfd, connfd;
static const char *device = "/dev/ttyS0";

volatile int fd;
struct Data data;
pthread_t id2;
char c;
char msg[50];
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

int count=0;

int main(int argc, char *argv[])
{
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
        if (num_bytes1 < 0)
        {
            printf("Error reading2: %s", strerror(errno));
            
        }
        else if(num_bytes1==21){
          count++;
          
          printf("count %d . Received message---- : %s\n", count, msg);
        }
          fflush (stdout);
        }
	//	}
    usleep (50000) ;
	}
	return 0;
}
