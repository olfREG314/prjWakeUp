#include<NTPClient.h>//update time from ntp
#include<ESP8266WiFi.h>//connect to wifi
#include<WiFiUdp.h>//receive data from internet
#include<WakeOnLan.h>
//===wifi 2.4Gh credentials================
const char* ssid="fbi_open";
const char* pswd="qwertyxiop";
//==================================
WiFiUDP UDP;
const char *MACAddress="2c:f0:5d:67:04:78";
WakeOnLan WOL(UDP);
//5.5*60*60=19800
const long utcOffsetInSec=19800;
//provide time
NTPClient timeClient(UDP,"pool.ntp.org",utcOffsetInSec);
//===no-> day==============================
String week[7]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
//global var
int hh,mm,ss,day;
bool alreadyTriggered=false;

// ===cal Time===========================
void calCurrentTime(){
    mm+=1;
    ss=0;
    //just to be sure
    //update time every 5 minute
    if(mm==5){
        syncTime();
    }
    if(mm==60){
        hh+=1;
        mm=0;
    }
    if(hh==24){
        day+=1;
        hh=0;
        alreadyTriggered=false;
    }
}

void printCurrentTime(){
    Serial.println(String(hh)+":"+mm+":"+ss+":"+week[day]);
}

void connectToWifi(){
    WiFi.begin(ssid,pswd);
    Serial.println(String("Connecting to Wifi ")+String(ssid));
    while(WiFi.status()!=WL_CONNECTED){
        Serial.print(".");
        digitalWrite(LED_BUILTIN,HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN,LOW);
        delay(500);
    }
    Serial.println(" connected to "+String(ssid));
    Serial.print("local IP\t");
    Serial.println(WiFi.localIP());
    // Serial.print("local port\t");
    // Serial.println(UDP.localPort());
    //===getting UTC+5:30 form ntp=====
    syncTime();
}
void syncTime(){
    timeClient.update();
    Serial.print("local time\t");
    Serial.println(week[timeClient.getDay()]+" "+timeClient.getFormattedTime());
    // ===set initial time from ntp=====
    hh=timeClient.getHours();
    mm=timeClient.getMinutes();
    ss=timeClient.getSeconds();
    day=timeClient.getDay();
}

void wakeMyPC(){
    for(int i=0;i<3;++i){
        WOL.sendMagicPacket(MACAddress);//to port 9
        Serial.println("sent wakeup signal");
    }
}

void setup() {
    pinMode(LED_BUILTIN,OUTPUT);//LOW->ON HIGH->OFF GPIO-1
    pinMode(14,OUTPUT);//small led
    pinMode(13,OUTPUT);//triggering relay IN4
    digitalWrite(13,HIGH);
    Serial.begin(115200);//baud rate 
    delay(2000);//avoiding serial output missing
    connectToWifi();
    
    
}

void loop() {
    delay(500);
    digitalWrite(14,LOW);
    ss+=1;
    delay(500);
    digitalWrite(14,HIGH);
    if(ss==60){
        calCurrentTime();
        Serial.println("ss update");
    }
    if(hh==3 && mm==0 && alreadyTriggered==false){
        printCurrentTime();
        digitalWrite(13,LOW);
        wakeMyPC();
        alreadyTriggered=true;
    }
    //===turnON wifi indicator=====
    if(WiFi.status()!=WL_CONNECTED){
        Serial.println("-Disconnected-");
        connectToWifi();
    }
    printCurrentTime();
}
