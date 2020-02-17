
#include <SoftwareSerial.h>

#include <DHT.h>
SoftwareSerial mySerial(1, 0); // RX, TX

const int DHTPIN = 2;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
char InString[5];
String receiv;
DHT dht(DHTPIN, DHTTYPE);
const String key = "1997.";
const String data = "da*";
String stringDetect="";
String stringData="";


int i = 0;
float ccc = 7.4;
int num = 12;
int keynode=97;
int keyData=99;
char output[10];

int flag=0;
String t =String(ccc);
void setup() {
stringDetect+='{';
stringDetect+='"';
stringDetect+="keynode";
stringDetect+='"';
stringDetect+=':';
//mystring+='"'+t+'"';
stringDetect+=keynode;
stringDetect+='}';
  
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
//  strcpy(si,"deptrai32");
//  sprintf(temp, "%f", 3.12);
//  strcat(si,temp);
//  Serial.println(temp);
  while (mySerial.available())
  {
 
    
   //memcpy(&test,&doc,sizeof(test));
//   Serial.println(test);
    receiv = mySerial.readString();
    Serial.println(receiv);
    if (key.equals(receiv))
    {
     
        Serial.println("key yes");
     //   receiv = "";
//        dtostrf(ccc, 9, 3, output);
//        output[0]='9';
//        output[1]='9';
        mySerial.println(stringDetect);
        flag++;
       // memset(&output, '/0', sizeof(output));
     
    }
    
    if (data.equals(receiv))
    {
      Serial.println("req data");
      receiv = "";
      stringData+='{';
      stringData+='"';
      stringData+="keynode";
      stringData+='"';
      stringData+=':';
      //mystring+='"'+t+'"';
      stringData+=keyData;
      stringData+=",";
      stringData+='"';
      stringData+="Temp";
      stringData+='"';
      stringData+=':';
      stringData+=ccc;
      stringData+='}';
      mySerial.println(stringData);
      stringData="";
      Serial.println();
    }

  }
}
