#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* Set these to your desired credentials. */
char *AP_ssid = "AP_chas";
char *AP_passkey = "12345678";
char temp[50];
char ssid[50];
char pass[50];
int status = WL_IDLE_STATUS;

boolean alreadyConnected[2] ; // whether or not the client was connected previously

WiFiServer server(6000);
WiFiClient client;
IPAddress ip(192, 168, 20, 13); 

IPAddress gateway(192, 168, 20, 1);
IPAddress subnet(255, 255, 255, 0);
#define Config_Led D2

void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.println("\nConfiguring access point...");
    WiFi.mode(WIFI_AP);
    WiFi.config(ip, gateway, subnet);
    WiFi.softAP(AP_ssid, AP_passkey);
    delay(10000);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    Serial.println("\nStarting server...");
    // start the server:
    server.begin(); 
    Serial.println("\nServer Started.");
}

void SetUpWifi(char ssid[50], char pass[50]) {
  int checkWifi = 0;
  Serial.println(pass);
  Serial.println(ssid);
  WiFi.begin(ssid, pass);           
  while (WiFi.status() != WL_CONNECTED) {
    checkWifi ++;
    if(checkWifi > 20) break;
    delay(500);
    Serial.print(".");
  }
  if(checkWifi > 20) {
    client.print("wifi not available");
    return;
  }
  Serial.println("connected");
  server.stop();
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
      while(client.available()>0){
        // read data from the connected client
        Serial.write(client.read()); 
      }
      //Send Data to connected client
      while(Serial.available()>0)
      {
        // read the bytes incoming from the client:
        String data = client.readString();
        // echo the bytes back to the client:
        data.toCharArray(temp,sizeof(temp));
        Serial.println(temp);
        for (int j=0; j< sizeof(temp); j++)
        {
          if(temp[j]=='.') {
          for(int k=0 ; k < sizeof(temp) - j; k++) {
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
        SetUpWifi(ssid,pass);
      }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}
