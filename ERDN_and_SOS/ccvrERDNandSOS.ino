const int pin = 27;
const int led = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(pin) == 1){
    digitalWrite(led, HIGH);
    Serial.println("open");
    delay(1000);
  }else {
    Serial.println("short");
    digitalWrite(led, LOW);
    delay(1000);
  }
}
