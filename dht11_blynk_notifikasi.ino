#define BLYNK_AUTH_TOKEN "2Rh9F8HSs8DR0fy1qAKvmSp4UcrEyvsC"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 

#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "ISA NETWORK";  // type your wifi name
char pass[] = "Polinema2021";  // type your wifi password

#define DHTPIN D5          // Mention the digital pin where you connected 
#define DHTTYPE DHT11     // DHT 11  
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;



void setup(){
   Serial.begin(115200);
  
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  
  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.print("|| Kelembapan : ");
  Serial.println(h);


  if(t > 30){
    //Blynk.email("isamahfudi08@gmail.com", "Alert", "Temperature over 30C!");
    Blynk.logEvent("notifikasi","Suhu diatas 30 Derajat celcius");
  }
}
