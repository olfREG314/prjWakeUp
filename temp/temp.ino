void setup(){
  Serial.begin(9600);
  Serial.println("start");
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
}
void loop(){
  digitalWrite(13,LOW);
  Serial.println("ON");
  delay(4000);
  digitalWrite(13,HIGH);
  Serial.println("OFF");
  delay(2000);
  
}