#include<ArduinoIoTCloud.h>
#include<Arduino_ConnectionHandler.h>
// #include "secret.h"
//every board is given unique id
const char DEVICE_LOGIN_NAME[]="6cfc6d58-f0e3-4c01-92cd-9175f93c6bb8";

const char SSID[]="fbi_open";
const char PSWD[]="qwertyxiop";
const char DEVICE_KEY[]="OP6A5BB5G8QTJSFNYRWE";

void onAlarmSetChange();
void onLampChange();
void onAlarmMinuteChange();

int alarmMinute;
int alarmSet;
bool lamp;
String arduinoTime;

void initProperties(){
    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(lamp,READWRITE,ON_CHANGE,onLampChange);
    ArduinoCloud.addProperty(alarmSet, READWRITE, ON_CHANGE, onAlarmSetChange);
    ArduinoCloud.addProperty(alarmMinute, READWRITE, ON_CHANGE, onAlarmMinuteChange);
    ArduinoCloud.addProperty(arduinoTime, READ, ON_CHANGE, NULL);
    

}
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID,PSWD);