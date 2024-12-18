#define RX1_PIN 44  // Define the RX pin for UART1
#define TX1_PIN 43  // Define the TX pin for UART1

struct LedInfo {
  int gpio;
  String function;
};

LedInfo ledPins[] = {
  { 45, "C-LP" }, { 48, "C-ALP" }, { 36, "C-LF" }, { 35, "C-RG" }, { 38, "C-TRACK" }, { 37, "C-PANTO" }, 
  { 40, "CAM-7" }, { 39, "CAM-8" }, { 42, "CAM-9" }, { 41, "CAM-10" }, { 1, "CAM-11" }, { 2, "CAM-12" }
};

const int Active_power_pin = 4;
const int Alternative_power_pin = 5;
const int Active_power_led = 47;
const int Alternative_power_led = 21;
const int Hooter_pin = 16;
const int Acknowledgement_Button = 6;
const int Panic_button = 7;
const int Panic_led = 12;
const int M_LP = 13;
const int M_ALP = 14;
const int RDAS = 11;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);
  initializePins();
}

void loop() {
  String input = "";
  boolean Acknowledgement_Status = false;
  
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n');
  } else if (Serial1.available() > 0) {
    input = Serial1.readStringUntil('\n');
  }

  input.trim();

  if (input.length() > 0) {
    processInput(input);
  }

  checkPowerStatus();
  checkPanicButton();
  checkAcknowledgementButton(Acknowledgement_Status);
  
  delay(1000);
}

void initializePins() {
  pinMode(Acknowledgement_Button, INPUT_PULLUP);
  pinMode(Active_power_pin, INPUT_PULLUP);
  pinMode(Alternative_power_pin, INPUT_PULLUP);
  pinMode(Panic_button, INPUT_PULLUP);
  pinMode(Panic_led, OUTPUT);
  pinMode(Active_power_led, OUTPUT);
  pinMode(Alternative_power_led, OUTPUT);
  pinMode(Hooter_pin, OUTPUT);
  pinMode(M_LP, OUTPUT);
  pinMode(M_ALP, OUTPUT);
  pinMode(RDAS, OUTPUT);

  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    pinMode(ledPins[i].gpio, OUTPUT);
  }
}

void processInput(String input) {
  String led_string = "CAM";
  int atIndex = input.indexOf('@');
  
  if (atIndex != -1) {
    String firstPart = input.substring(0, atIndex);
    String secondPart = input.substring(atIndex + 1);

    if (firstPart.indexOf(led_string) != -1) {
      led_process(firstPart, secondPart);
    } else {
      processCommands(firstPart, secondPart);
    }
  } else {
    Serial.println("Delimiter '@' not found in the input.");
  }
}

void processCommands(String firstPart, String secondPart) {
  if (firstPart == "Hooter") {
    digitalWrite(Hooter_pin, secondPart == "on" ? HIGH : LOW);
    Serial1.println(String(Hooter_pin) + " is " + (secondPart == "on" ? "on" : "off"));
  } else if (firstPart == "M_LP") {
    digitalWrite(M_LP, secondPart == "on" ? HIGH : LOW);
    Serial1.println("M_LP is " + (secondPart == "on" ? "on" : "off"));
  } else if (firstPart == "M_ALP") {
    digitalWrite(M_ALP, secondPart == "on" ? HIGH : LOW);
    Serial1.println("M_ALP is " + (secondPart == "on" ? "on" : "off"));
  } else if (firstPart == "RDAS") {
    digitalWrite(RDAS, secondPart == "on" ? HIGH : LOW);
    Serial1.println("RDAS is " + (secondPart == "on" ? "on" : "off"));
  }
}

void checkPowerStatus() {
  digitalWrite(Active_power_led, digitalRead(Active_power_pin) == 1 ? LOW : HIGH);
  Serial1.println(digitalRead(Active_power_pin) == 1 ? "Active_power is off" : "Active_power is on");

  digitalWrite(Alternative_power_led, digitalRead(Alternative_power_pin) == 1 ? LOW : HIGH);
  Serial1.println(digitalRead(Alternative_power_pin) == 1 ? "Alternative_power is off" : "Alternative_power is on");
}

void checkPanicButton() {
  digitalWrite(Panic_led, digitalRead(Panic_button) == 0 ? LOW : HIGH);
  Serial1.println(digitalRead(Panic_button) == 0 ? "Panic button is not pressed" : "Panic button is pressed");
}

void checkAcknowledgementButton(boolean &Acknowledgement_Status) {
  if (digitalRead(Acknowledgement_Button) == 0) {
    Acknowledgement_Status = true;
  }
  Serial1.println(Acknowledgement_Status);
}

void led_process(String firstPart, String secondPart) {
  int ledIndex = firstPart.substring(3).toInt() - 1;
  
  if (ledIndex >= 0 && ledIndex < sizeof(ledPins) / sizeof(ledPins[0])) {
    digitalWrite(ledPins[ledIndex].gpio, secondPart == "on" ? HIGH : LOW);
    Serial1.print(ledPins[ledIndex].gpio);
    Serial1.print(": ");
    Serial1.println(secondPart == "on" ? "HIGH" : "LOW");
    Serial1.print("Function: ");
    Serial1.println(ledPins[ledIndex].function);
  } else {
    Serial1.print("Unknown command for ");
    Serial1.println(firstPart);
  }
}
