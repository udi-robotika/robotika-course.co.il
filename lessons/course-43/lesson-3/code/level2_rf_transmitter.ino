/*
  Robotika – Lesson 3 · LEVEL 2a: RF433 remote (the TRANSMITTER)
  --------------------------------------------------------------
  With IR the remote came ready-made. With RF433 we BUILD the remote too:
  a second Arduino with 4 buttons that sends a radio code for each direction.

  RF = radio waves at 433 MHz. Unlike IR it does NOT need line of sight,
  goes through walls, and reaches much farther. Still one-way (remote -> car).

  This board is the REMOTE. The car runs level2_rf_car.ino.

  Library: "rc-switch" (Manage Libraries -> search "rc-switch")

  Wiring (this remote board):
    RF transmitter (FS1000A): DATA -> D10   VCC -> 5V   GND -> GND
                              (a ~17 cm wire as antenna helps a lot)
    Buttons to GND, using the internal pull-ups:
      UP -> D2   DOWN -> D3   LEFT -> D4   RIGHT -> D5   (STOP = no button)
*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

const int TX_PIN = 10;
const int BTN_UP = 2, BTN_DOWN = 3, BTN_LEFT = 4, BTN_RIGHT = 5;

// the codes we agree on with the car (any distinct numbers)
const long CODE_STOP = 10, CODE_UP = 11, CODE_DOWN = 12, CODE_LEFT = 13, CODE_RIGHT = 14;

void setup() {
  mySwitch.enableTransmit(TX_PIN);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void loop() {
  long code = CODE_STOP;                        // nothing pressed -> tell car to stop
  if (digitalRead(BTN_UP)    == LOW) code = CODE_UP;
  else if (digitalRead(BTN_DOWN)  == LOW) code = CODE_DOWN;
  else if (digitalRead(BTN_LEFT)  == LOW) code = CODE_LEFT;
  else if (digitalRead(BTN_RIGHT) == LOW) code = CODE_RIGHT;

  mySwitch.send(code, 24);   // send the 24-bit code over the air
  delay(100);                // ~10 times a second
}
