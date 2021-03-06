 #include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <RF24.h>
#include <string.h>
#include <DHT.h>

#define tempPin 4

int gas_din=6;                                            // pin 2 ket noi Dout
int gas_ain=A0; 

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
volatile float valueTemp = -1;
volatile float valueHum = -1;
volatile int valueGas = -1;
float valuePres = -1;
float valuePres_temp = -1;
float valueRain = -1;

char data[255];
void setup()
{
  Serial.begin(9600);
  radio.begin();
    
  pinMode(gas_din,INPUT);          
  pinMode(gas_ain,INPUT);
  
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
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
  if(digitalRead(6)==0){
     valueGas = analogRead(A0);
  }else valueGas = -1;
=======
>>>>>>> Stashed changes
  valueGas=0;
  valueHum=0;
  valuePres=0;
  valueRain=0;
  
  Serial.print("KhiGasDigital:");
  Serial.println(digitalRead(gas_din));
  if(digitalRead(gas_din)==0){
     valueGas = analogRead(gas_ain);
     Serial.print("KhiGas:");
     Serial.println(valueGas);
  }
<<<<<<< Updated upstream
=======
>>>>>>> b79f3102b35ea12e9b617b45103ee04898c766a1
>>>>>>> Stashed changes
  valueTemp =  dht.readTemperature();
  valueHum = dht.readHumidity();
  valuePres = bmp.readPressure();
  if (valuePres_temp == valuePres)
  {
    valuePres = -1;
  }else {
    valuePres_temp = valuePres;
  }
  /*temp*/

  stringData = "{\"id\":"; 
  stringData += idNode;
  stringData += ",\"type\":";
  stringData += 1;
  stringData += ",\"t\":\"";
  stringData += valueTemp;
  stringData += "\"}";

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
  stringData += ",\"h\":\"";
  stringData += valueHum;
  stringData += "\"}";

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