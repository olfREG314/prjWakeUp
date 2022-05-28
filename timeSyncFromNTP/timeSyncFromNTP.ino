#include<NTPClient.h>
#include<ESP8266WiFi.h>
#include<WiFiUdp.h>

const char *ssid="fbi_open";
const char *password="qwertyxiop";
//utc +5:30=5.5*60*60=19800
const long utcOffsetInSec=19800;
char daysOfTheWeek[7][12]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
//define NTP client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org",utcOffsetInSec) ;
void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid,password);
    
    while(WiFi.status()!=WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    Serial.print("Connected");
    timeClient.begin();
}

void loop() {
  timeClient.update();
  Serial.print("\n");
  Serial.print(String(daysOfTheWeek[timeClient.getDay()])+", "+timeClient.getHours()+", "+timeClient.getMinutes()+", "+timeClient.getSeconds()+", "+timeClient.getFormattedTime());
  delay(2000);
    
}
