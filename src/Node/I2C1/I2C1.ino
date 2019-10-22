// Include Arduino Wire library for I2C
#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 5
char key[6];
const String keyNode = "1997.";
const String dataNode = "da*";
// Define string with response to Master
int i=0;
volatile int flag=0;
String receiveGw="";
String stringDetect="";
String stringData="";
int keynode=97;
int keyData=99;




void setup() {
  
  stringDetect+='{';
  stringDetect+='"';
  stringDetect+="keynode";
  stringDetect+='"';
  stringDetect+=':';
  //mystring+='"'+t+'"';
  stringDetect+=keynode;
  stringDetect+='}';
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
  
  // Function to run when data requested from master
  Wire.onRequest(requestEvent); 
  
  // Function to run when data received from master
  Wire.onReceive(receiveEvent);
  
  // Setup Serial Monitor 
  Serial.begin(9600);
  Serial.println("I2C Slave Demonstration");
}
 
void receiveEvent() {
 
  // Read while data received
  while (0 < Wire.available()) {
    key[i] = Wire.read();
    receiveGw+=key[i];
   // Serial.println(key[i]);
   
    if(key[i]=='.')
    {
      Serial.println(receiveGw);
      if (keyNode.equals(receiveGw))
      {
        Serial.println("exactly KEYNode");
        flag=0;
      }
      i=0;
      memset(&key,'/0',sizeof(key));
      receiveGw="";
    }

    
    if(key[i]=='*')
    {
     
        Serial.println(receiveGw);
        if (dataNode.equals(receiveGw))
        {
          Serial.println("exactly DataNode");
          flag=1;
        }
        i=0;
        receiveGw="";
        memset(&key,'/0',sizeof(key));
    }
         i++;
  }
  
  // Print to Serial Monitor
  Serial.println("Receive event");
}

void requestEvent() {
  // Send response back to Master
  if(flag==0){
     Serial.println("Respone keyNode");
   //  String stringDe="97";
     Wire.println(stringDetect);
  }else
  {
      stringData+='{';
      stringData+='"';
      stringData+="keynode";
      stringData+='"';
      stringData+=':';
      //mystring+='"'+t+'"';
      stringData+=keyData;
      stringData+='}';
      Wire.println(stringData);
      stringData="";
 // Print to Serial Monitor
      Serial.println("Respone dataNode");
  }
 
}
 
void loop() {
 
  // Time delay in loop
  delay(50);
}
