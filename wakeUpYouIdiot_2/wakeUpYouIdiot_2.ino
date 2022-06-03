#include "thingProperties_wakeUpYouIdiot_2.h"
#include<NTPClient.h>//update time from ntp
#include<ESP8266WiFi.h>//connect to wifi
#include<WiFiUdp.h>//receive data from internet
#include <WakeOnLan.h>//start computer

WiFiUDP UDP;

//5.5*60*60=19800
const long utcOffsetInSec=19800;
//provide time
NTPClient timeClient(UDP,"pool.ntp.org",utcOffsetInSec);

//global var
//===no-> day=====
String week[7]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
int hh,mm,ss,day;
int alreadyTriggered=0;
bool _lastLampSignal=false;
int _ss=0,_lastMM=0;

int _alarmMinute=23;
int _alarmSet=23;

// ===initiate WOL=====
WakeOnLan WOL(UDP);

// ===Update time from NTP=====
void updateTime(){
    timeClient.update();
    Serial.print("local time\t");
    Serial.println(week[timeClient.getDay()]+" "+timeClient.getFormattedTime());
    // ===set initial time from ntp=====
    hh=timeClient.getHours();
    mm=timeClient.getMinutes();
    ss=timeClient.getSeconds();
    day=timeClient.getDay();
}

// ===cal Time===========================
void calCurrentTime(){
    // if(ss==60){
        mm+=1;
        ss=0;
    // }
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
String getCurrentTime(){
    return (String(hh)+":"+mm+":"+ss+":"+week[day]);
}

// ===Wake Up My computer=====
void wakeMyPC(){
    const char *MACAddress="2c:f0:5d:67:04:78";
    WOL.sendMagicPacket(MACAddress);//to port 9
}

// ===change lamp light=====
void triggerLamp(bool isLamp){
    if(isLamp==true){
        Serial.println("lamp=true");
        pinMode(13,LOW);
    }
    if(isLamp==false){
        Serial.println("lamp=false");
        pinMode(13,HIGH);
    }
}

// ===One time run=====
void setup() {
    Serial.begin(115200);
    delay(6000);//wait for serial monitor to initialize

    // ===connect to arduino cloud=====
    initProperties();
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    
    while(WiFi.status()!=WL_CONNECTED){
        //doing nothing if network not connected
        delay(1000);
        }
    ArduinoCloud.update();
    
    // ===Initialize pin=====
    pinMode(LED_BUILTIN,OUTPUT);//HIGH is off, LOW is on ,GPIO1
    pinMode(LED_BUILTIN,HIGH);//HIGH is off, LOW is on ,GPIO1
    pinMode(14,OUTPUT);// small LED
    pinMode(13,OUTPUT);// relay IN4
    pinMode(13,HIGH);
    
    // WOL stuff
    delay(3000);
    // Serial.println("WOL_section");
    // WOL.calculateBroadcastAddress(WiFi.localIP(),WiFi.subnetMask());
    // Serial.println("WOL stuff done");

    //===getting UTC+5:30 form ntp=====
    timeClient.begin();
    updateTime();
    arduinoTime=String(hh)+" : "+mm+" : "+ss;
    _ss=ss+5;
    _lastMM=mm;
}

void loop() {
    digitalWrite(14,HIGH);
    delay(500);
    ss+=1;
    digitalWrite(14,LOW);
    delay(500);
    // ===update cloud output timr every 5 sec=====
    // if(ss>=_ss){
    //     _ss=ss+5;
    //     Serial.println("arduinoTime update");
    // }

    if(ss==60){
        calCurrentTime();
        arduinoTime=getCurrentTime(); 
        if(WiFi.status()!=WL_CONNECTED){
            while(WiFi.status()!=WL_CONNECTED){
                //doing nothing if network not connected
                Serial.println("disconnected");
                digitalWrite(LED_BUILTIN,HIGH);//wifi module light OFF
                delay(500);
                digitalWrite(LED_BUILTIN,LOW);//if connected it will be ON
                delay(500);
            }
            updateTime();
        }

    }
    ArduinoCloud.update();
    if((mm - _lastMM)>=5){
        // Serial.println("_lastMM update");
        _lastMM=mm;
        updateTime();
    }
    if(hh>=_alarmSet && mm>=_alarmMinute && alreadyTriggered==0){ //loop hole
        for(int i=0;i<3;++i){
            // Serial.println("wake up time has come");
            wakeMyPC();
            triggerLamp(!lamp);
            delay(500);
        }
        arduinoTime=getCurrentTime(); 
        alreadyTriggered = 1;
    }
    // printCurrentTime();
    // Serial.println(getCurrentTime());
}

// ===LAMP=====
void onLampChange(){
    // Serial.println("Lamp value updated");
    triggerLamp(lamp);
}
void onAlarmSetChange(){
    // Serial.println("alarmSet changed");
    pinMode(LED_BUILTIN,HIGH);
    delay(50);
    pinMode(LED_BUILTIN,LOW);
    _alarmSet=alarmSet;
    alreadyTriggered=0;
}
void onAlarmMinuteChange()  {
    // Serial.println("alarmMinute changed");
    pinMode(LED_BUILTIN,HIGH);
    delay(50);
    pinMode(LED_BUILTIN,LOW);
    _alarmMinute=alarmMinute;
    alreadyTriggered=0;
}
