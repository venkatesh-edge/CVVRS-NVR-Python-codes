Overview:

This project is an Arduino sketch that manages a system with multiple LEDs, a hooter, power status indicators, a panic button, and an acknowledgement button. It communicates via UART1 and serial communication. The sketch allows you to control and monitor various components and LEDs through serial commands.

Hardware Connections:

UART1 Pins
RX1_PIN: 44 (Receive pin for UART1)
TX1_PIN: 43 (Transmit pin for UART1)

1. LED Pins:

# Defined in an array with their functions


```cpp
struct LedInfo {
  int gpio;
  String function;
};

LedInfo ledPins[] = {
  { 45, "C-LP" }, { 48, "C-ALP" }, { 36, "C-LF" }, { 35, "C-RG" }, 
  { 38, "C-TRACK" }, { 37, "C-PANTO" }, { 40, "CAM-7" }, { 39, "CAM-8" }, 
  { 42, "CAM-9" }, { 41, "CAM-10" }, { 1, "CAM-11" }, { 2, "CAM-12" }
};
