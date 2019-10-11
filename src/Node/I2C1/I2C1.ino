// Include Arduino Wire library for I2C
#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 5
char key[6];
// Define string with response to Master
String answer = "97";
 int i=0;
 volatile int flag=0;
void setup() {
 
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
    Serial.println(key[i]);
  //  Serial.print("i=");
    //Serial.println(i);
   
    if(key[i]=='.')
    {
      flag=0;
        Serial.println("end line");
     //  Serial.println(key[i]);
     //   Wire.onRequest(requestEvent);
        i=0;
        memset(&key,'/0',sizeof(key));
    }

    
    if(key[i]=='*')
    {
        Serial.println("end line1");
      // Serial.println(key[i]);
      //  Wire.onRequest(requestEvent1);
      flag=1;
        i=0;
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
     Serial.println("Request key");
     Wire.write("97");
  }else
  {
      Wire.write("99");
  // Print to Serial Monitor
  Serial.println("Request data");
  }
 
  //delay(2000);
  // Print to Serial Monitor

  //return;
 
}
 
void loop() {
 
  // Time delay in loop
  delay(50);
}
