#define RX1_PIN 44  // Define the RX pin for UART1
#define TX1_PIN 43  // Define the TX pin for UART1

// Define the LED pins in an array
struct Cam_LedInfo {
  int gpio;
  String function;
};

const Cam_LedInfo ledPins[] = {
  { 45, "C-LP" }, { 48, "C-ALP" }, { 36, "C-LF" }, { 35, "C-RG" }, { 38, "C-TRACK" }, { 37, "C-PANTO" }, { 40, "CAM-7" }, { 39, "CAM-8" }, { 42, "CAM-9" }, { 41, "CAM-10" }, { 1, "CAM-11" }, { 2, "CAM-12" }
};

const int Spare_ledPins[] = { 15, 8, 18, 17 };
const int ledCount = sizeof(ledPins) / sizeof(ledPins[0]);
const int Spare_ledCount = sizeof(Spare_ledPins) / sizeof(Spare_ledPins[0]);

const int Active_power_pin = 4;
const int Active_power_led = 47;
const int Alternative_power_led = 21;
const int Hooter_pin = 16;
const int Acknowledgement_Button = 6;
const int Panic_button = 7;
const int Panic_led = 12;
const int M_LP = 13;
const int M_ALP = 14;
const int RDAS = 11;
const int MB = 9;
const int GPS = 10;
const int WIFI = 3;
const int GSM = 46;
const int speaker = 5;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);
  pinMode(39, HIGH);
  pinMode(Acknowledgement_Button, INPUT_PULLUP);
  pinMode(Active_power_pin, INPUT_PULLUP);
  pinMode(Panic_button, INPUT_PULLUP);
  pinMode(Panic_led, OUTPUT);
  pinMode(Active_power_led, OUTPUT);
  pinMode(Alternative_power_led, OUTPUT);
  pinMode(Hooter_pin, OUTPUT);
  pinMode(M_LP, OUTPUT);
  pinMode(M_ALP, OUTPUT);
  pinMode(RDAS, OUTPUT);
  pinMode(MB, OUTPUT);
  pinMode(GPS, OUTPUT);
  pinMode(WIFI, OUTPUT);
  pinMode(GSM, OUTPUT);
  pinMode(speaker, OUTPUT);

  // Set all LED pins as outputs
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i].gpio, OUTPUT);
  }

  for (int j = 0; j < Spare_ledCount; j++) {
    pinMode(Spare_ledPins[j], OUTPUT);
  }
}

void loop() {
  String input = getSerialInput();
  static boolean Acknowledgement_Status = false;

  if (input.length() > 0) {
    if (input.indexOf("@") != -1) {
      processInput(input);
    } else {
      float frequency = input.toFloat();
      if (frequency > 0) {
        tone(speaker, frequency);
      } else {
        noTone(speaker);
      }
    }
  }

  // ERDN CODE CONTINUOUS READ Active_power AND Alternative_power
  handlePowerStatus();

  // PANIC BUTTON CODE.
  handlePanicButton();

  // ACKNOWLEDGEMENT BUTTON CODE RETURNS BOOLEAN VALUE.
  handleAcknowledgementButton(Acknowledgement_Status);

  // Add a small delay to prevent flooding the serial monitor
  delay(500);
}

String getSerialInput() {
  if (Serial.available() > 0) {
    return Serial.readStringUntil('\n');
  } else if (Serial1.available() > 0) {
    return Serial1.readStringUntil('\n');
  }
  return "";
}

void processInput(String input) {
  input.trim();
  int atIndex = input.indexOf('@');
  if (atIndex != -1) {
    String firstPart = input.substring(0, atIndex);
    String secondPart = input.substring(atIndex + 1);

    if (firstPart.indexOf("CAM") != -1) {
      Cam_led_process(firstPart, secondPart);
    } else if (firstPart.indexOf("GPIO") != -1) {
      Spare_led_process(firstPart, secondPart);
    } else if (firstPart == "Hooter") {
      handleHooter(secondPart);
    } else if (firstPart == "M_LP") {
      handlePinControl(M_LP, "M_LP", secondPart);
    } else if (firstPart == "M_ALP") {
      handlePinControl(M_ALP, "M_ALP", secondPart);
    } else if (firstPart == "RDAS") {
      handlePinControl(RDAS, "RDAS", secondPart);
    } else if (firstPart == "MB") {
      handlePinControl(MB, "Mother board", secondPart);
    } else if (firstPart == "GPS") {
      handlePinControl(GPS, "GPS", secondPart);
    } else if (firstPart == "WIFI") {
      handlePinControl(WIFI, "WIFI", secondPart);
    } else if (firstPart == "GSM") {
      handlePinControl(GSM, "GSM", secondPart);
    } else {
      Serial.println("Unknown command.");
    }
  }
}

// control function for Hooter, M_LP, M_ALP, RDAS

void handlePinControl(int pin, String pinName, String state) {
  if (state == "on") {
    digitalWrite(pin, HIGH);
    Serial.println(pinName + " is on");
    Serial1.println(pinName + " is on");
  } else if (state == "off") {
    digitalWrite(pin, LOW);
    Serial.println(pinName + " is off");
    Serial1.println(pinName + " is off");
  }
}

// Hooter funcition ///////////////////////////////

void handleHooter(String state) {
  handlePinControl(Hooter_pin, "Hooter", state);
}

// ERDN power status ///////////////////////////////////////

void handlePowerStatus() {
  if (digitalRead(Active_power_pin) == LOW) {
    digitalWrite(Active_power_led, HIGH);
    digitalWrite(Alternative_power_led, LOW);
    Serial.println("Active power is on");
    Serial.println("Alternative power is off");
    Serial1.println("Active power is on");
    Serial1.println("Alternative power is off");
  } else {
    digitalWrite(Active_power_led, LOW);
    digitalWrite(Alternative_power_led, HIGH);
    Serial.println("Active power is off");
    Serial.println("Alternative power is on");
    Serial1.println("Active power is off");
    Serial1.println("Alternative power is on");
  }
}

// Panic Button .////////////////////////////

void handlePanicButton() {
  digitalWrite(Panic_led, (digitalRead(Panic_button) == LOW) ? LOW : HIGH);
  Serial.println((digitalRead(Panic_button) == LOW) ? "Panic button is not pressed" : "Panic button is pressed");
  Serial1.println((digitalRead(Panic_button) == LOW) ? "Panic button is not pressed" : "Panic button is pressed");
}

// Acknowledgment function ////////////////////////

void handleAcknowledgementButton(boolean Acknowledgement_Status) {
  if (digitalRead(Acknowledgement_Button) == HIGH) {
    Serial.println(Acknowledgement_Status);
    Serial1.println(Acknowledgement_Status);
  } else {
    Acknowledgement_Status = true;
    Serial.println(Acknowledgement_Status);
    Serial1.println(Acknowledgement_Status);
  }
}

// Cam led function ////////////////////////////

void Cam_led_process(String firstPart, String secondPart) {
  int ledIndex = firstPart.substring(3).toInt() - 1;
  if (ledIndex >= 0 && ledIndex < ledCount) {
    if (secondPart == "on") {
      digitalWrite(ledPins[ledIndex].gpio, HIGH);
      Serial.print(ledPins[ledIndex].gpio);
      Serial.println(": HIGH");
    } else if (secondPart == "off") {
      digitalWrite(ledPins[ledIndex].gpio, LOW);
      Serial.print(ledPins[ledIndex].gpio);
      Serial.println(": LOW");
    } else {
      Serial.print("Unknown command for ");
      Serial.println(ledPins[ledIndex].function);
      return;
    }
    Serial.print("Function: ");
    Serial.println(ledPins[ledIndex].function);
  }
}

// Spare led function /////////////////////////

void Spare_led_process(String firstPart, String secondPart) {
  int ledIndex = firstPart.substring(4).toInt() - 1;
  if (ledIndex >= 0 && ledIndex < Spare_ledCount) {
    if (secondPart == "on") {
      digitalWrite(Spare_ledPins[ledIndex], HIGH);
      Serial.print("spare ");
      Serial.print(ledIndex + 1);
      Serial.print(": ");
      Serial.println("on");
      Serial1.print("spare ");
      Serial1.print(ledIndex + 1);
      Serial1.print(": ");
      Serial1.println("on");
    } else if (secondPart == "off") {
      digitalWrite(Spare_ledPins[ledIndex], LOW);
      Serial.print("spare ");
      Serial.print(ledIndex + 1);
      Serial.print(": ");
      Serial.println("off");
    } else {
      Serial.print("Unknown command for spare ");
      Serial.println(ledIndex + 1);
    }
  }
}
