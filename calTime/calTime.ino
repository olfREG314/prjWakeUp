// calculate time

int second, minute, hour, day;
void setup() {
pinMode(LED_BUILTIN,OUTPUT);
  second = minute = hour = 0;
  Serial.begin(9600);
}

void loop() {
  //cal time
  ++second;
    
  if (second == 60) {
    ++minute;
    second = 0;
  }
  if (minute == 60) {
    ++hour;
    minute = 0;
  }
  if (hour == 24) {
    ++day;
    hour = 0;
  }
  // String currTime=String(second) +" : " +minute+" : "  +hour;
  Serial.println(String(hour)+" : "+minute+" : "+second);
  // Serial.println(currTime);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
}
