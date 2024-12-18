const int hooterPin = 16;

void setup() {
  // put your setup code here, to run once:
  pinMode(hooterPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  String input = " ";
  if(Serial.available() > 0){
    input = Serial.readStringUntil('\n');
    if(input == "Hooter@on" ){
      digitalWrite(hooterPin, HIGH);
    }else if (input == "Hooter@off") {
      digitalWrite(hooterPin, LOW);
    }
  }
}
