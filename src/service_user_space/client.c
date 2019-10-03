
// Write CPP code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <syslog.h>

#define MAX 80
#define PORT 6997
#define SA struct sockaddr

void delay(int number_of_seconds) {
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Stroing start time
  clock_t start_time = clock();

  // looping till required time is not acheived
  while (clock() < start_time + milli_seconds)
    ;
}

int main() {
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket create and varification
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
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
    syslog(LOG_INFO, "connection with the server failed...\n");
    exit(0);
  }
  else
    syslog(LOG_INFO, "connected to the server..\n");

  // function for chat
  while (1) {
    FILE *f = popen("mount | grep /dev/sdb1", "r");
    if (NULL != f)
    {
      if (EOF == fgetc(f))
      {
        puts("/dev/sdb1 is NOT mounted");
      }
      else
      {
        puts("/dev/sdb1 is mounted");
        write(sockfd, "usb", sizeof("usb"));
      }
      /* close the command file */
      pclose(f);
    }
    delay(1000);
  }

  // close the socket
  close(sockfd);
}
