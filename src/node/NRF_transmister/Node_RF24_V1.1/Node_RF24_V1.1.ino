 #include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <RF24.h>
#include <string.h>
#include <DHT.h>

#define tempPin 4

Adafruit_BMP280 bmp; // I2C

const int DHTTYPE = DHT11;
//create an RF24 object
RF24 radio(9, 10);  // CE, CSN
DHT dht(tempPin, DHTTYPE);

/*khi  ga A0*/

//address through which two modules communicate.
const byte address[6] = "00001";

String stringData = "";
int idNode = 96;
volatile float valueTemp = 0.0;
volatile float valueHum = 0.0;
volatile int valueGas = 0;
float valuePres = 0.0;
float valueRain = 0.0;

char data[255];
void setup()
{
  Serial.begin(9600);
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();
  dht.begin();/*init sensor temp*/
  bmp.begin();
//  while(!bmp.begin()) {
//    Serial.println("Not Connect BMP");
//    delay(500);
//  }
  bmp.begin();
}
void loop()
{
  valueGas = analogRead(A0);
  valueTemp =  dht.readTemperature();
  valueHum = dht.readHumidity();
  //Serial.println(valueGas);
  valuePres = bmp.readPressure();
  Serial.println(valuePres);
  /*temp*/

  stringData = "{\"id\":"; 
  stringData += idNode;
  stringData += ",\"type\":";
  stringData += 1;
  stringData += ",\"t\":";
  stringData += valueTemp;
  stringData += '}';

  stringData.toCharArray(data, sizeof(data));
  radio.write(&data,sizeof(data));
  
  stringData = "";
  memset(&data,'\0',sizeof(data));
  delay(400);

/*humidity*/
  stringData = "{\"id\":";
  stringData += idNode;
  stringData += ",\"type\":";
  stringData += 2;
  stringData += ",\"h\":";
  stringData += valueHum;
  stringData += '}';

  stringData.toCharArray(data, sizeof(data));
  radio.write(&data,sizeof(data));
  
  stringData = "";
  memset(&data,'\0',sizeof(data));
  delay(400);
  
/*Gas air*/
  stringData = "{\"id\":";
  stringData += idNode;
  stringData += ",\"type\":";
  stringData += 3;
  stringData += ",\"g\":";
  stringData += valueGas;
  stringData += '}';

  stringData.toCharArray(data, sizeof(data));
  radio.write(&data,sizeof(data));

  stringData = "";
  memset(&data,'\0',sizeof(data));
  delay(400);

/*pressure*/
  stringData = "{\"id\":";
  stringData += idNode;
  stringData += ",\"type\":";
  stringData += 4;
  stringData += ",\"p\":";
  stringData += valuePres;
  stringData += '}';
  
  stringData.toCharArray(data, sizeof(data));
  radio.write(&data,sizeof(data));

  stringData = "";
  memset(&data,'\0',sizeof(data));
  delay(400);

}

////Include Libraries
////#include <SPI.h>
////#include <nRF24L01.h>
//#include <RF24.h>
//#include <string.h>
//
////create an RF24 object
//RF24 radio(9, 10);  // CE, CSN
//
////address through which two modules communicate.
//const byte address[6] = "00001";
//
//String stringData = "";
//int idNode = 96;
//float valueTemp = 0.0;
//float valueHum = 0.0;
//float valueGas = 0.0;
//float valuePres = 0.0;
//float valueRain = 0.0;
//
//char data[255];
//void setup()
//{
//  Serial.begin(9600);
//  radio.begin();
//  
//  //set the address
//  radio.openWritingPipe(address);
//  
//  //Set module as transmitter
//  radio.stopListening();
//}
//void loop()
//{
//  /*temp*/
//
//  stringData = "{\"id\":";
//  stringData += idNode;
//  stringData += ",\"type\":";
//  stringData += 1;
//  stringData += ",\"t\":";
//  stringData += valueTemp;
//  stringData += '}';
//
//  stringData.toCharArray(data, sizeof(data));
//  radio.write(&data,sizeof(data));
//  
//  stringData = "";
//  memset(&data,'\0',sizeof(data));
//  delay(400);
//
///*humidity*/
//  stringData = "{\"id\":";
//  stringData += idNode;
//  stringData += ",\"type\":";
//  stringData += 2;
//  stringData += ",\"h\":";
//  stringData += valueHum;
//  stringData += '}';
//
//  stringData.toCharArray(data, sizeof(data));
//  radio.write(&data,sizeof(data));
//  
//  stringData = "";
//  memset(&data,'\0',sizeof(data));
//  delay(400);
//  
///*Gas air*/
//  stringData = "{\"id\":";
//  stringData += idNode;
//  stringData += ",\"type\":";
//  stringData += 3;
//  stringData += ",\"g\":";
//  stringData += valueGas;
//  stringData += '}';
//
//  stringData.toCharArray(data, sizeof(data));
//  radio.write(&data,sizeof(data));
//
//  stringData = "";
//  memset(&data,'\0',sizeof(data));
//  delay(400);
//
///*pressure*/
//  stringData = "{\"id\":";
//  stringData += idNode;
//  stringData += ",\"type\":";
//  stringData += 4;
//  stringData += ",\"p\":";
//  stringData += valuePres;
//  stringData += '}';
//  
//  stringData.toCharArray(data, sizeof(data));
//  radio.write(&data,sizeof(data));
//
//  stringData = "";
//  memset(&data,'\0',sizeof(data));
//  delay(400);
//
//}
