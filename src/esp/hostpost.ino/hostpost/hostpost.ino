#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "data.h"

/* Set these to your desired credentials. */
const char* mqtt_server = "test.mosquitto.org";
const char *AP_ssid = "AP_chas";
const char *AP_passkey = "12345678";
char temp[50];
char ssid[50];
char pass[50];
long lastMsg = 0;
char mess[50];
int value = 0;
int status = WL_IDLE_STATUS;
WiFiClient client1;
PubSubClient client_mqtt(client1);
struct Data data;
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
      // Once connected, publish an announcement...
      client_mqtt.publish("sdt_wifi", "hello");
      // ... and resubscribe
      client_mqtt.subscribe("sdt_wifi");
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
        client_mqtt.loop();

       long now = millis();
       if (now - lastMsg > 2000) {
          client_mqtt.publish("sdt_wifi", "hello");
          delay(5000);
       }
    }  
  }
}
