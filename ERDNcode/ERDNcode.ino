const int ErdnPin = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(ErdnPin, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(ErdnPin) == 1){
    Serial.println("V2@off");
    delay(1000);
  }else if (digitalRead(ErdnPin) == 0) {
    Serial.println("V2@on");
    delay(1000);
  }
}
