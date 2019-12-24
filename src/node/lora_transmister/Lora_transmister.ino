#include <SPI.h>
#include <LoRa.h>

int counter = 0;
#define RST 9 //RST of Lora connected to pin 9
String stringData = "";
int idNode = 95;
float valueTemp = 0.0;
float valueHum = 0.0;
float valueGas = 0.0;
float valuePres = 0.0;
float valueRain = 0.0;
void setup() {
  Serial.begin(9600);
  pinMode(RST, OUTPUT); 
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
  while (!Serial);

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
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

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