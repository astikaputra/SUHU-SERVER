#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

String apiWritekey = "E14TQ50Z0A5K95CG"; // replace with your THINGSPEAK WRITEAPI key here
const char* ssid = "TP-Link_B108"; // your wifi SSID name
const char* password = "70328943" ;// wifi pasword
const char* server = "api.thingspeak.com";

#include <dht.h>
#define dataPin 14 // Defines pin number to which the sensor is connected D6 nodemcu
dht DHT; // Creats a DHT object
WiFiClient client;
 
void setup ()  
{   
  
  Serial.begin(115200);  
 
lcd.begin(); 
   lcd.backlight();
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
  
}  
   
void loop ()  
{  
  int readData = DHT.read22(dataPin); // Reads the data from the sensor jika menggunakan dht 11 tinggal ganti angka 22 menjadi 11
  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the 
  if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(t);
           tsData +="&field2=";
           tsData += String(h);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);
 
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
     Serial.println(" uploaded to Thingspeak server....");
    
  }
   client.stop();

  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
  lcd.clear();

}
