#include <SPI.h>
#include <LoRa.h>

int counter = 0;
#define RFM95_RST 9 //RST of Lora connected to pin 9
String stringData="";
int keyData = "97";
int value = 7.9;
void setup() {
  Serial.begin(9600);
  pinMode(RFM95_RST, OUTPUT); 
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
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

  stringData+='{';
  stringData+='"';
  stringData+="keynode";
  stringData+='"';
  stringData+=':';
  //mystring+='"'+t+'"';
  stringData+=keyData;
  stringData+=",";
  stringData+='"';
  stringData+="Temp";
  stringData+='"';
  stringData+=':';
  stringData+=value;
  stringData+='}';

  Serial.println(stringData);
  LoRa.println(stringData);
  stringData="";
  
  LoRa.endPacket();

  counter++;
  Serial.println("end");

  delay(1000);
}