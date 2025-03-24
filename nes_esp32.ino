/*
Hi! It's open source code for make connection between nes controller and pc, raspberry, etc.
You need to connect your nes controller like this:

  1             2
___________________
\ o o o o o o o o /
 \ o o o o o o o /
  ---------------
   3     4     5
  
  1 - Data (Pin 14)
  2 - GND (GND)
  3 - PWR (3v3)
  4 - Latch (Pin 12)
  5 - Clock (Pin 13)

Also you need to choose Tools -> USB Mode -> USB OTG in Arduino IDE

Best wishes =)
*/

#define NES_LATCH  12
#define NES_CLOCK  13
#define NES_DATA   14
#define NES_DATA_2 15

#include <USB.h>
#include <USBHIDGamepad.h>

// Doesn't work yet
#define GAMEPAD_COUNT 2

USBHIDGamepad Gamepad;

uint8_t buttons[GAMEPAD_COUNT] = {0,0};
uint8_t buttonsPrev[GAMEPAD_COUNT] = {0,0};
uint8_t gpPins[GAMEPAD_COUNT] = {NES_DATA, NES_DATA_2};

void NesButtons() {
  for (uint8_t gp = 0; gp < GAMEPAD_COUNT; gp++) {
    buttons[gp] = 0;
  }
  // Latch controller state
  digitalWrite(NES_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(NES_LATCH, LOW);

  // Get all 8 button states
  for (uint8_t i = 0; i < 8; i++) {
    for(uint8_t gp=0; gp<GAMEPAD_COUNT; gp++) {
      buttons[gp] |= (!digitalRead(gpPins[gp]) << (7 - i)); // Read DATA
    }
    digitalWrite(NES_CLOCK, LOW);
    delayMicroseconds(6);
    digitalWrite(NES_CLOCK, HIGH);
    delayMicroseconds(6);
  }
}

void setup() {
  pinMode(NES_LATCH, OUTPUT);
  pinMode(NES_CLOCK, OUTPUT);
  pinMode(NES_DATA, INPUT);
  pinMode(NES_DATA_2, INPUT);

  // Serial.begin(115200);

  Gamepad.begin();
  USB.begin();
}

void loop() {
  NesButtons();
  // Serial.println(buttons[0], BIN);

  for(uint8_t gp=0; gp<GAMEPAD_COUNT-1; gp++){
    // if (buttons[gp] != buttonsPrev[gp]){
      for (uint8_t i = 0; i < 8; i++){
        pushBtn(buttons[gp], i);
      }
      // buttonsPrev[gp] = buttons[gp];
    // }
  }

  delay(10);
}

void pushBtn(uint8_t buttons, uint8_t btn){
  if (buttons & (1 << (7 - btn))) {
    Gamepad.pressButton(btn);
  } else {
    Gamepad.releaseButton(btn);
  }
}