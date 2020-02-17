/*Lib https://circuits4you.com/2019/01/25/interfacing-dht11-with-nodemcu-example/?fbclid=IwAR2cm5vnDtMBI-93_6LzdT8ldhOP9fjGqImZxsKGyQ1xY_MCa6DYMvZm7wQ*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include "DHTesp.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define DHTpin 14    //D5 of NodeMCU is GPIO14

Adafruit_BMP280 bme; // I2C
DHTesp dht;

int gas_din=D0;                                            // pin 2 ket noi Dout
int gas_ain=A0; 

String stringData = "";
const int idNode = 94;
volatile float valueTemp = -1;
volatile float valueHum = -1;
volatile int valueGas = -1;
volatile float valuePres = -1;
volatile float valuePres_temp = -1;

/* Set these to your desired credentials. */
const char* mqtt_server = "test.mosquitto.org";
const char *AP_ssid = "AP_chas";
const char *AP_passkey = "12345678";
char temp[60];
char ssid[50];
char pass[50];
long lastMsg = 0;
char mess[50];
int value = 0;
int status = WL_IDLE_STATUS;
WiFiClient client1;
PubSubClient client_mqtt(client1);
char *msg;

boolean alreadyConnected[2] ; // whether or not the client was connected previously

WiFiServer server(6000);
IPAddress ip(192, 168, 20, 13); 

IPAddress gateway(192, 168, 20, 1);
IPAddress subnet(255, 255, 255, 0);
#define Config_Led D2
#define host "192.168.1.1"
#define port 6997

void setup()
{
    delay(1000);
    Serial.begin(115200);
    
    pinMode(gas_din,INPUT);          
    pinMode(gas_ain,INPUT); 
    dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
    
    Serial.println();
    Serial.println("\nConfiguring access point...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, AP_passkey);
    delay(10000);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    Serial.println("\nStarting server...");
    // start the server:
    server.begin(); 
    Serial.println("\nServer Started.");
    client_mqtt.setServer(mqtt_server, 1883);
    client_mqtt.setCallback(callback);
}

bool SetUpWifi(char ssid[50], char pass[50]) {
  int checkWifi = 0;
  WiFi.softAPdisconnect (true);
  server.stop();
  Serial.println(pass);
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);           
  while (WiFi.status() != WL_CONNECTED) {
    checkWifi ++;
    if(checkWifi > 20) {
      // client.print("wifi not available");
      // WiFi.mode(WIFI_AP);
      return false;  
    }
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("connected");
  return true;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client_mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "sdt_esp8266";
    // Attempt to connect
    if (client_mqtt.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client_mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{
   WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      if(client.available()>0){
        // read data from the connected client
        // Serial.write(client.read()); 
        String temp = client.readString();
        // echo the bytes back to the client:
        // temp.toCharArray(temp,sizeof(temp));
        Serial.println(temp);
        for (int j=0; j< sizeof(temp); j++)
        {
          if(temp[j]=='.') {
          for(int k=0 ; k < sizeof(temp); k++) {
            if(temp[k+j+1] == '\n'){
              pass[k]= '\0';
              break;
            }else pass[k]=temp[k+j+1];
          }
            break;
         }
          if(temp[j] == '\n'){
            ssid[j]= '\0';
          }else ssid[j]=temp[j];
        }
        if (SetUpWifi(ssid,pass)) break;
      }
    }
    Serial.println("Client disconnected");  
    while(1){
      if (!client_mqtt.connected()) {
        reconnect();
      }
      
      bme.begin();
      Serial.println(digitalRead(gas_din));
      if(digitalRead(gas_din)==0){
         valueGas = analogRead(gas_ain);
         Serial.print("KhiGas:");
         Serial.println(valueGas);
      }
      
      valueTemp =  dht.getTemperature();
      valueHum = dht.getHumidity();
      valuePres = bme.readPressure();
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
      client_mqtt.loop();
      memset(temp,0,60);
      stringData.toCharArray(temp, stringData.length()+1);
      Serial.println(temp);
      long now = millis();
      if (now - lastMsg > 2000) {
         lastMsg = now;
         client_mqtt.publish("data_wifi", temp);
         delay(5000);
      }
    }  
  }
}