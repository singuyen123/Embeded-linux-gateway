
#include <SPI.h>
#include <LoRa.h>

int counter = 0;
#define RFM95_RST 9 //RST of Lora connected to pin 9
void setup() {
  Serial.begin(9600);
    pinMode(RFM95_RST, OUTPUT); 
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(20);
    LoRa.setSignalBandwidth(62.5E3);
 LoRa.setSpreadingFactor(7);
  LoRa.setSyncWord(0x12);
 LoRa.setPreambleLength(6);
 // LoRa.enableCrc();
 LoRa.setOCP(240);
  LoRa.setCodingRate4(8);
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  Serial.println("end");

  delay(1000);
}

//#include <SPI.h>
//#include <LoRa.h>
//
//#define RFM95_RST 9 //RST of Lora connected to pin 9
//int counter = 0;
//
//void setup() {
//  Serial.begin(9600);
//  // Reset LoRa Module 
//  pinMode(RFM95_RST, OUTPUT); 
//  digitalWrite(RFM95_RST, LOW);
//  delay(10);
//  digitalWrite(RFM95_RST, HIGH);
//  delay(10);
//  while (!Serial);
//  
//  Serial.println("LoRa Sender");
//
//  if (!LoRa.begin(433E6)) {
//    Serial.println("Starting LoRa failed!");
//    while (1);
//  }
//  LoRa.setSignalBandwidth(62.5E3);
//  LoRa.setSpreadingFactor(12);
//  LoRa.setTxPower(18);
//  LoRa.setSyncWord(0x12);
//  LoRa.setPreambleLength(6);
//  LoRa.enableCrc();
//  LoRa.setCodingRate4(8);
//
//  
//}
//
//void loop() {
//  Serial.print("Sending packet: ");
//  Serial.println(counter);
//
//  // send packet
//  LoRa.beginPacket(0);
//  LoRa.print("hello");
//  LoRa.print(counter);
//  LoRa.endPacket(false);
//
//  counter++;
//
//  delay(1000);
//}


//
//#include <SPI.h> //Import SPI librarey 
//#include <RH_RF95.h> // RF95 from RadioHead Librarey 
//
//#define RFM95_CS 10 //CS if Lora connected to pin 10
//#define RFM95_RST 9 //RST of Lora connected to pin 9
//#define RFM95_INT 2 //INT of Lora connected to pin 2
//
//// Change to 434.0 or other frequency, must match RX's freq!
//#define RF95_FREQ 433.0
//
//// Singleton instance of the radio driver
//RH_RF95 rf95(RFM95_CS, RFM95_INT);
//
//void setup() 
//{
// 
////Initialize Serial Monitor
//  Serial.begin(9600);
//  
//// Reset LoRa Module 
//  pinMode(RFM95_RST, OUTPUT); 
//  digitalWrite(RFM95_RST, LOW);
//  delay(10);
//  digitalWrite(RFM95_RST, HIGH);
//  delay(10);
//
////Initialize LoRa Module
//  while (!rf95.init()) {
//    Serial.println("LoRa radio init failed");
//    while (1);
//  }
//  
//
// //Set the default frequency 434.0MHz
//  if (!rf95.setFrequency(RF95_FREQ)) {
//    Serial.println("setFrequency failed");
//    while (1);
//  }
//
//  rf95.setTxPower(18); //Transmission power of the Lora Module
//}
//
//char value = 48;
//
//void loop()
//{
//  Serial.print("Send: \n");
//  char radiopacket[1] = {char(value)};
//  rf95.send((uint8_t *)radiopacket, 1);
//
//    
//  delay(1000);
//  value++;
//  if (value > '9')
//  value = 48;
//}
