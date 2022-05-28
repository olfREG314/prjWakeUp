/*
connect to wifi
*/
#include <ESP8266WiFi.h> //module loaded automatically
#include <WakeOnLan.h>
#include <WiFiUdp.h>
WiFiUDP UDP;
WakeOnLan WOL(UDP);

const char *MACAddress = "2c:f0:5d:67:04:78";

const char* ssid="fbi_open";
const char* password="qwertyxiop";
void setup() {
    Serial.begin(9600);
  delay(3000);//for avoiding serial output missing
    WiFi.begin(ssid,password);
    Serial.println("Connecting to");
    Serial.print(ssid);Serial.println("...");
    //wait for the wifi to connect
    for(int i=0;WiFi.status()!=WL_CONNECTED;++i){
      delay(1000);
      Serial.print(String(i)+" ");
    }
    Serial.println('\n');
    Serial.println("Connection established");
    Serial.println("IP adderss:\t");
    Serial.println(WiFi.localIP());
    WOL.sendMagicPacket(MACAddress);
}

void loop() {
    
}
