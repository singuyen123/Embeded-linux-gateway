
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
        Serial.print("Chuoi nhan duoc: ");
        Serial.println(receiveGw);
        if (key.equals(receiveGw))
        {
          process = true;
         // receiveGw="";
          Serial.println("key yes");
         //flag=1;  
        }
        else{
          Serial.println("---------------------------------");
          process=false;
        }
        receiveGw="";
        
         fflush(stdin);
        Serial.println("End line");
      }
}


void loop (void) {
   while (process) {
      //process = false; //reset the process
      //SPI.detachInterrupt();
//      if (data.equals(receiveGw) && flag==1  )
//      {
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
      //}   
      //receiveGw="";
      Serial.println("End compare");
        stringData="";
   // SPI.attachInterrupt(); 
      fflush(stdin);
   }

  

   
}
