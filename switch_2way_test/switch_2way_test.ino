#include "thingProperties.h"
void setup() {
    Serial.begin(9600);
    delay(5000);
    initProperties();
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();

    // ===
    pinMode(13,OUTPUT);

}

void loop() {
    ArduinoCloud.update();
    delay(1000);
}

void onLampChange(){
    Serial.println("Lamp value updated");
    if(lamp==true){
        Serial.println("lamp=true");
        pinMode(13,LOW);
    }
    if(lamp==false){
        Serial.println("lamp=false");
        pinMode(13,HIGH);
    }
}