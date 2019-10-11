#include <SoftwareSerial.h>

#include <DHT.h>
SoftwareSerial mySerial(2, 3); // RX, TX

const int DHTPIN = 2;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
char InString[5];
char keyData[3];
String receiv;
DHT dht(DHTPIN, DHTTYPE);
const String key="1997.";
const String data="da*";
int i=0;
int flag=0;
void setup() {
  Serial.begin(9600);
  dht.begin();     
  // Khởi động cảm biến
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}
 
void loop() {
 /* float h = dht.readHumidity();    //Đọc độ ẩm
  float t = dht.readTemperature(); //Đọc nhiệt độ
  Serial.println("IOT");
  Serial.print("Nhiet do: ");
  Serial.println(t);               //Xuất nhiệt độ
  Serial.print("Do am: ");
  Serial.println(h);               //Xuất độ ẩm
  
  Serial.println();//Xuống hàng
  */
   while(mySerial.available())
   {
        receiv = mySerial.readString();
        Serial.println(receiv);
        if(key.equals(receiv))
        {
          Serial.println("key yes");
          receiv="";
           mySerial.write("99");
          // memset(&receiv,'/0',sizeof(receiv));
          
        }
        if(data.equals(receiv))
        {
          Serial.println("req data");
          receiv="";
           mySerial.write("79");
          // memset(&receiv,'/0',sizeof(receiv));
        }        
    
   }
}
