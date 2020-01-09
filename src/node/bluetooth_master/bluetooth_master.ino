#include <SoftwareSerial.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <string.h>
#include <DHT.h>

#define tempPin 4

Adafruit_BMP280 bmp; // I2C

const int DHTTYPE = DHT11;

String stringData = "";
const int idNode = 97;
volatile float valueTemp = 0.0;
volatile float valueHum = 0.0;
volatile int valueGas = 0;
volatile float valuePres = 0.0;

DHT dht(tempPin, DHTTYPE);
// SoftwareSerial mySerial(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();/*init sensor temp*/
 // bmp.begin();
  //mySerial.begin(115200);
}

void loop() {
      bmp.begin();
      valueGas = analogRead(A0);
      valueTemp =  dht.readTemperature();
      valueHum = dht.readHumidity();
      //Serial.println(valueGas);
      valuePres = bmp.readPressure();
     // Serial.println("{\"id\":37,\"data\":45}"); //gửi dữ liệu cho Arduino thứ 2

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
      stringData += '}';
    
      Serial.println(stringData);
      stringData = "";
      valuePres = 0.0;
      valueGas = 0;
      valueHum = 0.0;
      valueTemp = 0.0;
      delay(1000);

}
