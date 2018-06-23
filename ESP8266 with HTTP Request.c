 #include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial esp_(D2,D3); //RX, TX
const char* ssid = "MyWifi";
const char* password = "12345678@";
//byte server[] = {192,168,1,30};
const int httpPort = 80;
const char* host = "www.waterfilling.dsof-engineering.com";
int f=0,waktu,jumlah;
//======== For Web Parameter =======
int valve1=0,valve2=0,tank_level=0;
int total_barang=0, bottle_pos=0;
int valve1_buf=0,valve2_buf=0, tank_level_buf=0,total_barang_buf=0,bottle_pos_buf=0;
String data;
void setup() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 1);
  Serial.begin(9600);
  esp_.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(".");}
  Serial.println(WiFi.localIP());
  digitalWrite(D4, 0);
}

void loop(){
  //while (Serial.available()) {
  if(esp_.available()){
    data = esp_.readString();
    
    //Serial.print(data);
    f=1;
  }
  valve1=getValue(data,':',0).toInt();
  valve2=getValue(data,':',1).toInt();
  tank_level=getValue(data,':',2).toInt();
  total_barang=getValue(data,':',3).toInt();
  bottle_pos=getValue(data,':',4).toInt();
  if(bottle_pos_buf == 250){ bottle_pos_buf=0; }
  if(total_barang >= total_barang_buf && bottle_pos >= bottle_pos_buf){
    total_barang_buf=total_barang;
    bottle_pos_buf=bottle_pos;
//    String data2=String(data);
//    int first = data2.indexOf(':');
//    int second= data2.indexOf(':',first+1);
//    String waktu = data2.substring(0,first);
//    String jumlah = data2.substring(first+1,second);
//    Serial.print(valve1);
//    Serial.print(":");
//    Serial.println(valve2);
//    Serial.print(":");
//    Serial.print(tank_level);
//    Serial.print(":");
//    Serial.print(total_barang);
//    Serial.print(":");
//    Serial.print(bottle_pos);
    WiFiClient client;
//    if (client.connect(server, 80)) {
//      client.print("GET /c/project/konveyor2/add?");
//      client.print("waktu=");
//      client.print(waktu);
//      client.print("&jumlah=");
//      client.println(jumlah);
//      client.stop();  
//      }
    if (client.connect(host, 80)) {
      //Serial.println("connected");
//      String url = "/konveyor/add?";
      String url = "/update_data.php?";
      url += "valve1=";
      url += valve1;
      url += "&valve2=";
      url += valve2;
      url += "&tank_level=";
      url += tank_level;
      url += "&total_barang=";
      url += total_barang;
      url += "&bottle_pos=";
      url += bottle_pos;
      
//      url += "waktu="; 
//      url += waktu;
//      url += "&jumlah="; 
//      url += jumlah;

      //Using HTTP Request to send message
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");  
      //Serial.println("oke");
      }
    f=0;
    }
    
    //Serial.read();
    //String second = Serial.readStringUntil(':');
    //Serial.print(first);
    //Serial.print(">>>>");
    
  //  
    
  //}
  delay(50);
}

String getValue(String data, char separator, int index){
 int found = 0;
 int strIndex[] = { 0, -1  };
 int maxIndex = data.length()-1;
 for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
   found++;
   strIndex[0] = strIndex[1]+1;
   strIndex[1] = (i == maxIndex) ? i+1 : i;
   }
 }
 return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
  