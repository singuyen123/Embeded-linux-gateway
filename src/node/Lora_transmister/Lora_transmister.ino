#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <string.h>
#include <DHT.h>
#define tempPin 4
int rainSensor = 6;

Adafruit_BMP280 bmp; // I2C
const int DHTTYPE = DHT11;
DHT dht(tempPin, DHTTYPE);

int counter = 0;
#define RST 9 //RST of Lora connected to pin 9
String stringData = "";
int idNode = 95;

volatile float valueTemp = 0.0;
volatile float valueHum = 0.0;
volatile int valueGas = 0;
volatile float valuePres = 0.0;
volatile int valueRain = 0;

void setup() {
  Serial.begin(9600);
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
  LoRa.setOCP(240);
  LoRa.setCodingRate4(8);
  
  dht.begin();/*init sensor temp*/
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  bmp.begin();
  valueGas = analogRead(A2);
  valueTemp =  dht.readTemperature();
  valueHum = dht.readHumidity();
  //Serial.println(valueGas);
  valuePres = bmp.readPressure();
  valueRain = digitalRead(rainSensor);
  
  // send packet
  LoRa.beginPacket();

  stringData = "{\"id\":";
  stringData += idNode;
  /*temp*/
  stringData += ",\"t\":";
  stringData += valueTemp;
  /*humidity*/
  stringData += ",\"h\":";
  stringData += valueHum;
  /*gas air*/
  stringData += ",\"g\":";
  stringData += valueGas;
  /*pressure*/
  stringData += ",\"p\":";
  stringData += valuePres;
  /*rain*/
  stringData += ",\"r\":";
  stringData += valueRain;
  stringData += '}';

  Serial.println(stringData);
  LoRa.println(stringData);
  stringData = "";
  
  LoRa.endPacket();

  counter++;
  Serial.println("end");

  delay(200);
}
