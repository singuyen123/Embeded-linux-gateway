#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "data.h"

int status = WL_IDLE_STATUS;

struct Data data;
char *msg;
void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  WiFi.softAPdisconnect (true);
  WiFi.mode(WIFI_STA);
  WiFi.begin("@ss", "thomasthong");           
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client;
  if (!client.connect("192.168.1.197",6997)) {
        Serial.println("connection failed");
        //delay(5000);
      }
   if (client.connected()) {
      Serial.println("client send data to server");
      data.node = kWifi;
      memcpy(msg,&data,sizeof(data));
      client.write(msg,sizeof(data));
   }
   delay(1000);
}
