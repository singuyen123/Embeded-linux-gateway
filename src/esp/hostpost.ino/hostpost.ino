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
WiFiClient client[2];
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

void loop()
{
    unsigned int i;
    for (i= 0; i<2; i++)
    {
        // wait for a new client:
        if (!client[i])
        {
            client[i] = server.available();
        }
        else
        {
            if (client[i].status() == CLOSED)
            {
                client[i].stop();
                Serial.print("\nConnection closed on client: "  );
                Serial.println(i);
                Serial.println();
                alreadyConnected[i] = false;
            }

            else
            {
                if(!alreadyConnected[i]) // when the client sends the first byte, say hello:
                {
                    // clean out the input buffer:
                    client[i].flush();
                    Serial.print("\nNew client: ");
                    client[i].println("Hello, client !");
                    Serial.println(i);
                    alreadyConnected[i] = true;
                }

                if (client[i].available())
                {
                    // read the bytes incoming from the client:
                    String data = client[i].readString();
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
                    // client[i].print(data);
                    // echo the bytes to the server as well:
                    
                    Serial.println(pass);
                    Serial.println(ssid);
                    WiFi.begin(ssid, pass);

                    //Wifi connection
                    while (WiFi.status() != WL_CONNECTED) {
                      delay(500);
                       Serial.print(".");
                    }
                    Serial.println("connected");
                    client[i].print("wifi connected");
                }               
            }
        }
    }
}
