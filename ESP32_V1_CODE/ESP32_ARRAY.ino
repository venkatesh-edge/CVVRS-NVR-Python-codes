#define RX1_PIN 44  // Define the RX pin for UART1
#define TX1_PIN 43  // Define the TX pin for UART1


// Define the LED pins in an array
// const int ledPins[] = { 21, 48, 35, 37, 39, 41, 2, 14, 12, 10, 46, 8, 17, 47, 45, 36, 38, 40, 42, 1, 13, 11, 18, 16 };
// const int  ledPins[] = {45, 48, 36, 35, 38, 37, 40, 39, 42, 41, 1, 2};
// const int ledCount = sizeof(ledPins) / sizeof(ledPins[0]);

struct Cam_LedInfo {
  int gpio;
  String function;
};

Cam_LedInfo ledPins[] = {
  { 45, "C-LP" }, { 48, "C-ALP" }, { 36, "C-LF" }, { 35, "C-RG" }, { 38, "C-TRACK" }, { 37, "C-PANTO" }, { 40, "CAM-7" }, { 39, "CAM-8" }, { 42, "CAM-9" }, { 41, "CAM-10" }, { 1, "CAM-11" }, { 2, "CAM-12" }
};

const int Spare_ledPins[] = { 9, 10, 3, 18, 8, 17, 15 };
const int Spare_ledCount = sizeof(Spare_ledPins) / sizeof(Spare_ledPins[0]);

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
  Serial.println("Serial1 initialized 0");
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);
  Serial.println("Serial1 initialized 1");
  pinMode(39, HIGH);
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

  // Set all LED pins as outputs

  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    pinMode(ledPins[i].gpio, OUTPUT);
  }

  for (int j = 0; sizeof(Spare_ledPins) / sizeof(Spare_ledPins[0]); j++) {
    pinMode(Spare_ledPins[j], OUTPUT);
  }
}

void loop() {
  String input = " ";
  String cam_led_string = "CAM";
  String spr_led_string = "SPR";
  boolean Acknowledgement_Status = false;
  if (Serial.available() > 0) {
    // Read the incoming string from the serial buffer
    input = Serial.readStringUntil('\n');
  } else if (Serial1.available() > 0) {
    input = Serial1.readStringUntil('\n');
  }

  // Trim any leading or trailing whitespace from input
  input.trim();

  // Split the string using the '@' delimiter
  int atIndex = input.indexOf('@');
  if (atIndex != -1) {
    String firstPart = input.substring(0, atIndex);
    String secondPart = input.substring(atIndex + 1);

    // Print the split strings to the Serial Monitor
    if (firstPart.indexOf(cam_led_string) != -1) {
      Cam_led_process(firstPart, secondPart);
    } else if (firstPart.indexOf(spr_led_string) != -1) {
      Spare_led_process(firstPart, secondPart);
    } else if (firstPart == "Hooter") {
      if (secondPart == "on") {
        digitalWrite(Hooter_pin, HIGH);
        Serial1.println("Hooter is on");
      } else if (secondPart == "off") {
        digitalWrite(Hooter_pin, LOW);
        Serial1.println("Hooter is off");
      }
    } else if (firstPart == "M_LP") {
      if (secondPart == "on") {
        digitalWrite(M_LP, HIGH);
        Serial1.println("M_LP is on");
      } else if (secondPart == "off") {
        digitalWrite(M_LP, LOW);
        Serial1.println("M_LP is off");
      }
    } else if (firstPart == "M_ALP") {
      if (secondPart == "on") {
        digitalWrite(M_ALP, HIGH);
        Serial1.println("M_ALP is on");
      } else if (secondPart == "off") {
        digitalWrite(M_ALP, LOW);
        Serial1.println("M_ALP is off");
      }

    } else if (firstPart == "RDAS") {
      if (secondPart == "on") {
        digitalWrite(RDAS, HIGH);
        Serial1.println("RDAS is on");
      } else if (secondPart == "off") {
        digitalWrite(RDAS, LOW);
        Serial1.println("RDAS is off");
      }
    } else {
      Serial.println("Delimiter '@' not found in the input.");
    }
  }

  // ERDN CODE CONTINUOUS READ Active_power AND Alternative_power

  if (digitalRead(Active_power_pin) == 0) {
    digitalWrite(Active_power_led, HIGH);
    digitalWrite(Alternative_power_led, LOW);
    Serial.println("Active power is on");
    Serial.println("Alternative power is off");
  } else {
    digitalWrite(Active_power_led, LOW);
    digitalWrite(Alternative_power_led, HIGH);
    Serial.println("Active power is off");
    Serial.println("Alternative power is on");
  }

  // digitalWrite(Active_power_led, (digitalRead(Active_power_pin) == 1) ? LOW : HIGH);
  // Serial1.println((digitalRead(Active_power_pin) == 1) ? "Active_power is off" : "Active_power is on");

  // digitalWrite(Alternative_power_led, (digitalRead(Alternative_power_pin) == 1) ? LOW : HIGH);
  // Serial1.println((digitalRead(Alternative_power_pin) == 1) ? "Alternative_power is off" : "Alternative_power is on");

  // PANIC BUTTON CODE.

  digitalWrite(Panic_led, (digitalRead(Panic_button) == 0) ? LOW : HIGH);
  Serial1.println((digitalRead(Panic_button) == 0) ? "Panic button is not pressed" : "Panic button is pressed");

  // ACKNOWLEDGEMENT BUTTON CODE RETURNS BOOLEAN VALUE.

  if (digitalRead(Acknowledgement_Button) == 1) {
    // Acknowledgement_Status = false;
    Serial1.println(Acknowledgement_Status);
  } else if (digitalRead(Acknowledgement_Button) == 0) {
    Acknowledgement_Status = true;
    Serial1.println(Acknowledgement_Status);
  }

  // Add a small delay to prevent flooding the serial monitor
  delay(1000);
}


void Cam_led_process(String firstPart, String secondPart) {
  int ledIndex = firstPart.substring(3).toInt() - 1;
  if (ledIndex >= 0 && ledIndex < sizeof(ledPins) / sizeof(ledPins[0])) {
    if (secondPart == "on") {
      digitalWrite(ledPins[ledIndex].gpio, HIGH);
      Serial1.print(ledPins[ledIndex].gpio);
      Serial1.print(": ");
      Serial1.println("HIGH");
    } else if (secondPart == "off") {
      digitalWrite(ledPins[ledIndex].gpio, LOW);
      Serial1.print(ledPins[ledIndex].gpio);
      Serial1.print(": ");
      Serial1.println("LOW");
    } else {
      Serial1.print("Unknown command for ");
      Serial1.println(ledPins[ledIndex].function);
      return;
    }
    Serial1.print("Function: ");
    Serial1.println(ledPins[ledIndex].function);
  }
}

void Spare_led_process(String firstPart,String secondPart) {
  int ledIndex = firstPart.substring(3).toInt() - 1;

  if (ledIndex >= 0 && ledIndex < Spare_ledCount) {
    if (secondPart == "on") {
      digitalWrite(Spare_ledPins[0], HIGH);
      Serial1.print("spare ");
      Serial.print(ledIndex + 1);
      Serial.print(": ");
      Serial.print(secondPart);
    } else if (secondPart == "off") {
      digitalWrite(Spare_ledPins[0], LOW);
      Serial1.print("spare ");
      Serial.print(ledIndex + 1);
      Serial.print(": ");
      Serial.print(secondPart);
    }
  }
}

// void led_process(String firstPart, String secondPart) {
//   // Extract the LED number from the first part
//   int ledIndex = firstPart.substring(3).toInt() - 1;

//   // Check if the LED number is within the valid range
//   if (ledIndex >= 0 && ledIndex < ledCount) {
//     if (secondPart == "on") {
//       digitalWrite(ledPins[ledIndex], HIGH);
//       Serial1.print("Cam");
//       Serial1.print(ledIndex + 1);
//       Serial1.print(": ");
//       Serial1.print(secondPart);
//     } else if (secondPart == "off") {
//       digitalWrite(ledPins[ledIndex], LOW);
//       Serial1.print("Cam");
//       Serial1.print(ledIndex + 1);
//       Serial1.print(": ");
//       Serial1.print(secondPart);
//     }
//   }
// }




// Cam_LedInfo ledPins[] = {
//   {45, "C-LP"}, {48, "C-ALP"}, {36, "C-LF"}, {35, "C-RG"}, {38, "C-TRACK"}, {37, "C-PANTO"}, {40, "CAM-7"}, {39, "CAM-8"}, {42, "CAM-9"}, {41, "CAM-10"}, {1, "CAM-11"}, {2, "CAM-12"}
// }
