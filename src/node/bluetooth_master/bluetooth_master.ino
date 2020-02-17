#include <SoftwareSerial.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <string.h>
#include <DHT.h>

#define tempPin 4
int gas_din=10;                                            // pin 2 ket noi Dout
int gas_ain=A0; 

int gas_din=10;                                            // pin 2 ket noi Dout
int gas_ain=A0; 

int gas_din=10;                                            // pin 2 ket noi Dout
int gas_ain=A0; 

Adafruit_BMP280 bmp; // I2C

const int DHTTYPE = DHT11;

String stringData = "";
const int idNode = 97;
volatile int valueTemp = -1;
volatile int valueHum = -1;
volatile int valueGas = -1;
volatile int valuePres = -1;
volatile int valuePres_temp = -1;
DHT dht(tempPin, DHTTYPE);
// SoftwareSerial mySerial(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(gas_din,INPUT);          
  pinMode(gas_ain,INPUT);
  dht.begin();/*init sensor temp*/
}

void loop() {
      bmp.begin();
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
      if(digitalRead(10)==0){
         valueGas = analogRead(A0);
      }else valueGas = -1;       
=======
>>>>>>> Stashed changes
      valueGas=0;
      valueHum=0;
      valuePres=0;
      valueTemp=0;
      
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
     // Serial.println("{\"id\":37,\"data\":45}"); //gửi dữ liệu cho Arduino thứ 2

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
      stringData += '}';
    
      Serial.println(stringData);
      delay(1000);

}