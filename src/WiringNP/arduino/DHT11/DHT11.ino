#include <DHT.h>

const int DHTPIN = 2;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
String InString;
DHT dht(DHTPIN, DHTTYPE);
const String key="IOT@.";
 
void setup() {
  Serial.begin(9600);
  dht.begin();         // Khởi động cảm biến
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
/*  while(Serial.available())
  {
    char InChar;
    InChar = Serial.read();
    Serial.print(InChar);
  }
  */
   while(Serial.available())
   {
       //Serial.print("--Start--");
       InString=Serial.readString();
       int n=sizeof(InString);
       int flag=0;
         // Serial.println(n);
       for(int i=0;i<n-3;i++)
       {
           // Serial.print("Instring:");
             // Serial.println(InString[i]);
           //  && 'A'<key[i]<'Z' && 'A'<InString[i]<'Z'
          if(key[i]!=InString[i])
          {
            flag=1;
            //Serial.print("i=");
           // Serial.println(i);
          }
           // Serial.print("key:");
           // Serial.println(key[i]);
       }
      if(flag==0){
        //Serial.println("OK");
        Serial.write("right");
      }else
      {
       // Serial.println(flag);
        Serial.write("NoOk");
      }

        InString='\0';
        
       
    }
  
}
