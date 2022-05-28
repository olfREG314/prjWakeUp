#include <ArduinoIoTCloud.h>

const char DEVICE_LOGIN_NAME[]="6cfc6d58-f0e3-4c01-92cd-9175f93c6bb8";
const char DEVICE_KEY[]="OP6A5BB5G8QTJSFNYRWE";

void onLampChange();
bool lamp;

void initProperties(){
    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(lamp,READWRITE,ON_CHANGE,onLampChange);
}