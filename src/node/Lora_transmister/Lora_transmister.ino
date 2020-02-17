#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <string.h>
#include <DHT.h>
#define tempPin 4
int rainSensor = 6;

int gas_din=7;                                            // pin 2 ket noi Dout
int gas_ain=A2; 

Adafruit_BMP280 bmp; // I2C
const int DHTTYPE = DHT11;
DHT dht(tempPin, DHTTYPE);

int counter = 0;
#define RST 9 //RST of Lora connected to pin 9
String stringData = "";
int idNode = 95;

volatile float valueTemp = -1;
volatile float valueHum = -1;
volatile int valueGas = -1;
volatile float valuePres = -1;
volatile float valuePres_temp = -1;
volatile int valueRain = -1;

void setup() {
  Serial.begin(9600);
  
  pinMode(gas_din,INPUT);          
  pinMode(gas_ain,INPUT);
  pinMode(rainSensor,INPUT);
  pinMode(RST, OUTPUT); 
  
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
  //while (!Serial);

  //Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(20);
  LoRa.setSignalBandwidth(62.5E3);
  LoRa.setSpreadingFactor(7);
  LoRa.setSyncWord(0x12);
  LoRa.setPreambleLength(6);
  // LoRa.setOCP(240);
  LoRa.setCodingRate4(8);
  
  dht.begin();/*init sensor temp*/
  
}

void loop() {
  valueGas=0;
  valueHum=0;
  valuePres=0;
  valueRain=0;
  //Serial.print("Sending packet: ");
  //Serial.println(counter);
  bmp.begin();
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
  if(digitalRead(7)==0){
      valueGas = analogRead(A2);
  }else valueGas = -1;
=======
>>>>>>> Stashed changes
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
  valueRain = digitalRead(rainSensor);
  
  // send packet
  LoRa.beginPacket();

  stringData = "{\"id\":";
  stringData += idNode;
  /*temp*/
  stringData += ",\"t\":\"";
  stringData += valueTemp;
  /*humidity*/
  stringData += "\",\"h\":\"";
  stringData += valueHum;
  /*gas air*/
  stringData += "\",\"g\":";
  stringData += valueGas;
  /*pressure*/
  stringData += ",\"p\":";
  stringData += valuePres;
  /*rain*/
  stringData += ",\"r\":";
  stringData += valueRain;
  stringData += '}';

  //Serial.println(stringData);
  LoRa.println(stringData);
  stringData = "";
  
  LoRa.endPacket();

  counter++;
  //Serial.println("end");

  delay(200);
}