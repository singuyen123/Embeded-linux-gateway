gcc clientSPI.c -ljson-c -Wall -o spiclient2810 -lwiringPi -lpthread

//#include <SPI.h>
//char value[5];
//int i=0;
//const String key="1997.";
//
//
//char buff [50];
//volatile byte indx;
//volatile boolean process;
//
//void setup (void){
//    Serial.begin (9600);
//    // chon che do slave
//    SPCR |= bit (SPE);
//    // chuyen MISO sang master in, *slave out*
//    pinMode(MISO, OUTPUT);
//    indx = 0; // buffer empty
//    process = false;
//    // bat che do interrupts
//    SPI.attachInterrupt();
//    Serial.println("Receive via SPI: ");
// 
//}
// 
//// SPI interrupt routine
// 
//ISR (SPI_STC_vect){
//   byte c = SPDR; // read byte from SPI Data Register
//   if (indx < sizeof buff) {
//      buff [indx++] = c; // save data in the next index in the array buff
//      if (c == '\r') //check for the end of the word
//      process = true;
//   }
//   
////   if(i<5)
////   {
////    byte c = SPDR;
////    value[i]=SPDR;
////    Serial.write(c);
////    i++;
////   }
////   else{
////    i=0;
////    Serial.println(value);
////    String b=(String)(value);
////    if(b.equals(key))
////    {
////      byte C = 57;
////      SPDR = C;
////      Serial.println("right");
////    }else{
////      Serial.println("Not right");
////      byte  D= 55;
////      SPDR =D;
////      
////    }
////   }
//   
//    //Serial.write(c);
//}
// 
//void loop (void){
//   if (process) {
//      process = false; //reset the process
//      Serial.println (buff); //print the array on serial monitor
//      indx= 0; //reset button to zero
//   }
//    delay(10);
//}

//74HC595N
#include <SPI.h>
char buff [10];
volatile byte indx;
volatile boolean process;
String receiveGw="";

char responeKey[5]="55";
char responeData[5]="57";
const String key = "1997.";
const String data = "da*";

void setup (void) {
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
}
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
   if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      receiveGw+=char(c);
      if (c == '.'||c=='*') //check for the end of the word
      {
        process = true;
      }
      
   }
}

void loop (void) {
   if (process) {
      process = false; //reset the process
      Serial.println (buff); //print the array on serial monitor
      //Serial.print ("string");
      Serial.println (receiveGw);
      if (data.equals(receiveGw))
      {
        Serial.println("key yes");
      }
      memset(&buff, '\r', sizeof(buff));
      indx= 0; //reset button to zero
      receiveGw="";
      
   }
}



//#include <SPI.h>
//char value[5];
//int i=0;
//const String key="1997.";
//
//
//char buff [50];
//volatile byte indx;
//volatile boolean process;
//
//void setup (void){
//    Serial.begin (9600);
//    // chon che do slave
//    SPCR |= bit (SPE);
//    // chuyen MISO sang master in, *slave out*
//    pinMode(MISO, OUTPUT);
//    indx = 0; // buffer empty
//    process = false;
//    // bat che do interrupts
//    SPI.attachInterrupt();
//    Serial.println("Receive via SPI: ");
// 
//}
// 
//// SPI interrupt routine
// 
//ISR (SPI_STC_vect){
//   byte c = SPDR; // read byte from SPI Data Register
//   if (indx < sizeof buff) {
//      buff [indx++] = c; // save data in the next index in the array buff
//      if (c == '\r') //check for the end of the word
//      process = true;
//   }
//   
////   if(i<5)
////   {
////    byte c = SPDR;
////    value[i]=SPDR;
////    Serial.write(c);
////    i++;
////   }
////   else{
////    i=0;
////    Serial.println(value);
////    String b=(String)(value);
////    if(b.equals(key))
////    {
////      byte C = 57;
////      SPDR = C;
////      Serial.println("right");
////    }else{
////      Serial.println("Not right");
////      byte  D= 55;
////      SPDR =D;
////      
////    }
////   }
//   
//    //Serial.write(c);
//}
// 
//void loop (void){
//   if (process) {
//      process = false; //reset the process
//      Serial.println (buff); //print the array on serial monitor
//      indx= 0; //reset button to zero
//   }
//    delay(10);
//}


////////////////////////////////////////////////////2810////////////////???

//74HC595N
#include <SPI.h>
char buff [4];
volatile byte indx;
volatile boolean process;
String receiveGw="";

char responeKey[4]="55*";
char responeData[4]="77*";
const String key = "97.";
const String data = "da*";
 char buff1[10];
 char buff2[10];

volatile int flag=0;
volatile int i=0;
volatile int buf [5] = {55,57,55,57,55};
volatile int pos;

void setup (void) {

 
strcpy(buff1,"999");

strcpy(buff2,"797");
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
}
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
     // Serial.println(char(c));
      receiveGw+=char(c);
//      if(45<c<58 ||c==42)
//      {
//       // Serial.print(char(c));
//         buff [indx++] = c;
//      }
      
      if (char(c) == '.'||char(c)=='*') //check for the end of the word
      {
        process = true;
        //SPI.transfer(buff2,sizeof(buff2)+1);    
      }
}


void loop (void) {
   if (process) {
      process = false; //reset the process
      //Serial.print ("string");
      Serial.println (buff);
      if (data.equals(receiveGw))
      {
        Serial.println("data yes");
        Serial.println(receiveGw);
        SPDR=57;
        SPDR=56;
        SPDR=55;
//        flag=flag++%5;
//        while(flag!=0)
//        {
//          
//            SPDR=buf[i++];
//         
//        }
//          flag=0;
//          i=0;
  
        
      }
      if (key.equals(receiveGw))
      {
        Serial.println("key yes");
      
       
      }
      receiveGw="";
      indx=0;
      
   }
}



///*******************88send to gateway ok//////////////////////////////////


//74HC595N
#include <SPI.h>
char buff [4];
volatile byte indx;
volatile boolean process;
String receiveGw="";

char responeKey[4]="55*";
char responeData[4]="77*";
const String key = "7.";
const String data = "a.";
 char buff1[10];
 char buff2[10];

volatile int flag=0;
volatile int i=0;
char buf[5] = "1234";
volatile int pos;

void setup (void) {
SPIClass SPI;
 
strcpy(buff1,"999");

strcpy(buff2,"797");
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
   
}
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
     //Serial.println(char(c));
      receiveGw+=char(c);
//      if(45<c<58 ||c==42)
//      {
//       // Serial.print(char(c));
//         buff [indx++] = c;
//      }
      
      if (char(c) == '.') //check for the end of the word
      {
        process = true;
       // Serial.println("endline");
        //SPI.transfer(buff2,sizeof(buff2)+1);    
      }
}

char hello[5]="3678";
int k=0;
void loop (void) {
  //Serial.println("Start transfer");
  Serial.println(byte(hello[k]));
 // SPI.begin();
  SPDR=byte(hello[k]);
  k++;
  if(k==4)
  {
    Serial.println("endl");
    k=0;
  }
  SPI.endTransaction();
  //SPI.end();
 //SPI.transfer(buf,sizeof(buf)+1); 
// for(int i=0;i++;i<sizeof(buff1)+1)
// {
//   SPDR=buff1[i];
// }
// i=0;

//Serial.println("one");
//   SPDR=57;
//   Serial.println("two");
//   SPDR=56;
//      Serial.println("three");
//   SPDR=54;
   
  // delay(20);
  //delay(900);
   if (process) {
      process = false; //reset the process
      //Serial.print ("string");
      Serial.println (buff);
      if (data.equals(receiveGw))
      {
        Serial.println("data yes");
        Serial.println(receiveGw);
        SPDR=57;
        //SPDR=56;
        //SPDR=55;
//        flag=flag++%5;
//        while(flag!=0)
//        {
//          
//            SPDR=buf[i++];
//         
//        }
//          flag=0;
//          i=0;
  
        
      }
      if (key.equals(receiveGw))
      {
        Serial.println("key yes");
      
       
      }
      receiveGw="";
      indx=0;
      
   }
}

///************************///
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
#define speed 1000000
#define SS0 10 // GPIOG3 10
int sockfd, connfd;

static const char *API_key = "----------send ACK to SPI---------";
static const char *msg_send_data = "********send data to SPI************";
char requestKey[2] = {  '7', '.'};
char requestData[2] = {'a','.'};
char msgReceive[50];

volatile int fd;
int flagDetect = 0;

const int responeKey = 99;
const int responeData = 97;

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

void *threadfunction1(void *args)
{
    while (1)
    {
        printf("-----In Thread 1----\n");

      //  memset(&msgReceive, '\0', sizeof(msgReceive));

        pthread_mutex_lock(&mutexsum);
        int num_bytes1 = read(fd, &msgReceive, sizeof(msgReceive));
        pthread_mutex_unlock(&mutexsum);
        printf("Read %i bytes. Received message------ : %s\n", num_bytes1, msgReceive);
        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes1 < 0)
        {
            printf("Error reading2: %s", strerror(errno));
        }
        else
        {
            // printf("start read\n");
            // struct json_object *parsed_json;
            // struct json_object *keynode;

            // parsed_json = json_tokener_parse(msgReceive);
            // json_object_object_get_ex(parsed_json, "keynode", &keynode);

            // int checkResponse = json_object_get_int(keynode);
            // printf("checkResponse: %d\n", checkResponse);

            // printf("Read %i bytes. Received message***** : %s\n", num_bytes1, msgReceive);
            // if (checkResponse == responeKey)
            // {
            //     printf("Read %i bytes. Received message : %s\n", num_bytes1, msgReceive);
            //     printf("Detected---------------------------------------------------------------------- \n");
            //     flagDetect = 0;
            // }
            // else if (checkResponse == responeData)
            // {
            //     printf("Read %i bytes. Received message : %s\n", num_bytes1, msgReceive);
            //     printf("send data********************************************************************** \n");
            //     flagDetect = 0;
            // }
            // else
            // {
            //     flagDetect++;
            //     if (flagDetect > 10)
            //     {
            //         printf("No connect device\n");
            //         flagDetect = 0;
            //     }
            // }
        }


// int num_bytes1= wiringPiSPIDataRW(chanel,&msg_send_data,10);
//         printf("Read %i bytes. Received message------ : %s\n", num_bytes1, msgReceive);
      // usleep(5000);
       sleep(1);
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
    /*creating thread*/
    ret = pthread_create(&id1, NULL, &threadfunction1, NULL);
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

        // rc = poll(fds, 1, 500); // 1s time out
        // if (rc == 0)
        // {
        //   //  printf("start detect SPI>>>>>>>>>>>>>>\n");
        //     count = (count + 1) % 5;
        //     if (count == 0)
        //     {
        //         sendRequestToNode(requestKey, sizeof(requestKey), API_key);
        //     }
        //     else
        //     {
        //         sendRequestToNode(requestData, sizeof(requestData), msg_send_data);
        //     }
        // }
    }
    return (EXIT_SUCCESS);
}


/********************************************/


//74HC595N
#include <SPI.h>
char buff [4];
volatile byte indx;
volatile boolean process;
String receiveGw = "";

char responeKey[4] = "55*";
char responeData[4] = "77*";
const String key = "7.";
const String data = "a.";
char buff1[10];
char buff2[10];

volatile int flag = 0;
volatile int i = 0;
char buf[5] = "1234";
volatile int pos;

void setup (void) {
  SPIClass SPI;

  strcpy(buff1, "999");

  strcpy(buff2, "797");
  Serial.begin (115200);
  pinMode(MISO, OUTPUT); // have to send on master in so it set as output
  SPCR |= _BV(SPE); // turn on SPI in slave mode
  indx = 0; // buffer empty
  process = false;
  SPI.attachInterrupt(); // turn on interrupt

}
ISR (SPI_STC_vect) // SPI interrupt routine
{
  byte c = SPDR; // read byte from SPI Data Register
  //Serial.println(char(c));
  receiveGw += char(c);
  //      if(45<c<58 ||c==42)
  //      {
  //       // Serial.print(char(c));
  //         buff [indx++] = c;
  //      }

  if (char(c) == '.') //check for the end of the word
  {
    process = true;
    // Serial.println("endline");
    //SPI.transfer(buff2,sizeof(buff2)+1);
  }
}

char hello[8] = "{36789}";
int k = 0;
void loop (void) {
  //Serial.println("Start transfer");

  // SPI.transfer(&hello,6);
  // SPI.begin();
  while (1)
  {
    //SPI.begin();
    Serial.println(byte(hello[k]));
    SPDR = byte(hello[k]);
    delay(200);
    k++;
    if (k == 8)
    {
      Serial.println("endl");
      k = 0;
      //SPI.end();
      //delay(2000);
    }

  }
  // Serial.println("endl");
  k = 0;
  //  SPDR=byte(hello[k]);
  //  k++;
  //  if(k==4)
  //  {
  //    Serial.println("endl");
  //    k=0;
  //  }
  //  SPI.endTransaction();


  //SPI.end();
  //SPI.transfer(buf,sizeof(buf)+1);
  // for(int i=0;i++;i<sizeof(buff1)+1)
  // {
  //   SPDR=buff1[i];
  // }
  // i=0;

  //Serial.println("one");
  //   SPDR=57;
  //   Serial.println("two");
  //   SPDR=56;
  //      Serial.println("three");
  //   SPDR=54;

  // delay(20);
  //delay(900);
  if (process) {
    process = false; //reset the process
    //Serial.print ("string");
    Serial.println (buff);
    if (data.equals(receiveGw))
    {
      Serial.println("data yes");
      Serial.println(receiveGw);
      SPDR = 57;
      //SPDR=56;
      //SPDR=55;
      //        flag=flag++%5;
      //        while(flag!=0)
      //        {
      //
      //            SPDR=buf[i++];
      //
      //        }
      //          flag=0;
      //          i=0;


    }
    if (key.equals(receiveGw))
    {
      Serial.println("key yes");


    }
    receiveGw = "";
    indx = 0;

  }
}

/////////////////////////////////30/10///////////////////////

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
int flag1=0;
int flag2=0;
void *threadfunction1(void *args)
{
    while (1)
    {
        printf("-----In Thread 1----\n");
        // memset(&msgReceive, '\0', sizeof(msgReceive));
        //     int num_byte= wiringPiSPIDataRW(chanel,msgReceive,50);
        int num_byte = read(fd, msgReceive, sizeof(msgReceive));
        printf("Read %i bytes. Received message------ : %s\n", num_byte, msgReceive);
        usleep(500000);
      //  printf("-----------Received message11111111--- : %c\n", msgReceive[0]);
       // printf("-----------Received messa22222222--- : %c\n", msgReceive[1]);
        if (msgReceive[0] == '{')
        {
            flag1=1;
        }
        if(msgReceive[0]=='}'&& msgReceive1[0]=='{')
        {
            flag1=0;
             msgReceive1[ka] = msgReceive[0];
             printf("Received message11111111--- : %s\n", msgReceive1);
            ka = 0;
            memset(&msgReceive1, '\0', sizeof(msgReceive1));
        }
        if( flag1==1 && msgReceive[0] != '.' && msgReceive[0] != 'a' && msgReceive[0] != 'b' )
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
    /*creating thread*/
    ret = pthread_create(&id1, NULL, &threadfunction1, NULL);
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

        rc = poll(fds, 1, 10000); // 1s time out
        if (rc == 0)
        {
            //  printf("start detect SPI>>>>>>>>>>>>>>\n");
            // count = (count + 1) % 5;
            // if (count == 0)
            // {
                
                sendRequestToNode(requestKey, sizeof(requestKey), API_key);
            // }
            // else
            // {
            //     sendRequestToNode(requestData, sizeof(requestData), msg_send_data);
            // }
        }
    }
    return (EXIT_SUCCESS);
}

///////////////////////31/10//////////////////////////

//74HC595N
#include <SPI.h>

volatile boolean process;
String receiveGw="";

const String key = "b*";
const String data = "a*";

volatile int flag=0;
volatile int flag1=0;
volatile int i=0;

char hello[8] = "{36789}";
char hello1[8]="{12345}";
volatile int k = 0;

String stringData="";
int keynode=99;
int temp=37;
int keyData=97;

int countChar(String a)
{
  int i=0;
  int count=0;
  int flagcout=1;
  while(flagcout)
  {
  
  if(a[i]=='}')
  {
      flagcout=0;
  }
    count++;
    i++;
  }
  return count;
}

void setup (void) {

   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
   
}
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
     Serial.println(char(c));
     if(char(c)=='a' || char(c)=='b' || char(c)=='*' )
     {
      receiveGw += char(c);
     }
      if (char(c) == '*' ) //check for the end of the word
      {
        process = true;
        Serial.println(receiveGw);
        Serial.println("End line");
      }
}


void loop (void) {
   if (process) {
      process = false; //reset the process
      SPI.detachInterrupt();
      if (data.equals(receiveGw) && flag==1  )
      {
        Serial.println("data yes-----");
        receiveGw="";
          
        stringData  +=  '{';
        stringData  +=  '"';
        stringData  +=  "keynode";
        stringData  +=  '"';
        stringData  +=  ':';
        stringData  +=  keyData;
        stringData  +=  ",";
        stringData  +=  '"';
        stringData  +=  "Temp";
        stringData  +=  '"';
        stringData  +=  ':';
        stringData  +=  temp;
        stringData  +=  '}';
        
           flag1=1; 
         while (flag1)
        {
          SPDR = stringData[k];
          delay(50);
          k++;
          
          if (k == countChar(stringData))
          {
            Serial.println("endl");
            k = 0;
            flag1=0;
          }

        }
      }
      
      if (key.equals(receiveGw))
      {
        receiveGw="";
        Serial.println("key yes");
       flag=1;  
      }
      receiveGw="";
      Serial.println("End compare");
    SPI.attachInterrupt(); 
      
   }

  

   
}


///////////////////////////
