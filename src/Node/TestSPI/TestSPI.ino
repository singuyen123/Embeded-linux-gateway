
//74HC595N
#include <SPI.h>

volatile boolean process;
String receiveGw="";

const String key = "b.";
const String data = "a.";

volatile int flag=0;
volatile int i=0;

char hello[8] = "{36789}";
volatile int k = 0;

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
      receiveGw+=char(c);
      if (char(c) == '.') //check for the end of the word
      {
        process = true;
   
      }
}


void loop (void) {
   if (process) {
      process = false; //reset the process
      if (data.equals(receiveGw))
      {
        Serial.println("data yes");
        Serial.println(receiveGw);   
        
      }
      if (key.equals(receiveGw))
      {
        Serial.println("key yes");
        Serial.println(receiveGw);
        flag=1; 
       
      }
      receiveGw="";
      
   }

    while (flag)
        {
          //Serial.println(sizeof(hello));
          
          Serial.println(byte(hello[k]));
          SPDR = byte(hello[k]);
          delay(200);
          k++;
          
          if (k == (sizeof(hello)))
          {
            Serial.println("endl");
            k = 0;
          //  flag=0;
          }

        } 

   
}
