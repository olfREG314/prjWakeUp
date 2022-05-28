#include<NTPClient.h>//update time from ntp
#include<ESP8266WiFi.h>//connect to wifi
#include<WiFiUdp.h>//receive data from internet
// #include "thingProperties.h" //connection to Arduino Cloud
//===wifi 2.4Gh credentials================
const char* ssid="fbi_open";
const char* pswd="qwertyxiop";
//==================================
WiFiUDP UDP;
//5.5*60*60=19800
const long utcOffsetInSec=19800;
//provide time
NTPClient timeClient(UDP,"pool.ntp.org",utcOffsetInSec);
//===no-> day==============================
String week[7]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
//global var
int hh,mm,ss,day;
// ===cal Time===========================
void calCurrentTime(){
    ss+=1;
    if(ss==60){
        mm+=1;
        ss=0;
    }
    if(mm==60){
        hh+=1;
        mm=0;
    }
    if(hh==24){
        day+=1;
        hh=0;
    }
}
// print current time
void printCurrentTime(){
    Serial.println(String(hh)+":"+mm+":"+ss+":"+week[day]);
}

void setup() {
    pinMode(LED_BUILTIN,OUTPUT);//LOW->ON HIGH->OFF GPIO-1
    pinMode(14,OUTPUT);//small led
    pinMode(13,OUTPUT);//triggering relay IN4
    digitalWrite(13,HIGH);
    delay(5000);//avoiding serial output missing
    Serial.begin(9600);//baud rate
    // ===Connect to Wifi=====================
    WiFi.begin(ssid,pswd);
    Serial.println(String("Connecting to Wifi ")+String(ssid));
    while(WiFi.status()!=WL_CONNECTED){
        Serial.print(".");
        digitalWrite(LED_BUILTIN,LOW);
        delay(500);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(500);
    }
    Serial.println(" connected to "+String(ssid));
    Serial.print("local IP\t");
    Serial.println(WiFi.localIP());
    // Serial.print("local port\t");
    // Serial.println(UDP.localPort());
    //===turnON wifi indicator=====
    if(WiFi.status()==WL_CONNECTED){
        digitalWrite(LED_BUILTIN,LOW);
    }
    //===getting UTC+5:30 form ntp=====
    timeClient.begin();
    timeClient.update();
    Serial.print("local time\t");
    Serial.println(week[timeClient.getDay()]+" "+timeClient.getFormattedTime());
    // ===set initial time from ntp=====
    hh=timeClient.getHours();
    mm=timeClient.getMinutes();
    ss=timeClient.getSeconds();
    day=timeClient.getDay();
    //===connect to Arduino Cloud=====
    // initProperties();//not doing may be need to go deep down
}

void loop() {
    digitalWrite(14,HIGH);
    calCurrentTime();
    if(hh==0 && mm==8){
        printCurrentTime();
        digitalWrite(13,LOW);
    }
    delay(500);
    digitalWrite(14,LOW);
    delay(500);
}
