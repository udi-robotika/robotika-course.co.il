/*
  Robotika – Lesson 3 · FINALE (demo, we finish it next lesson):
  Drive the car straight from your PHONE over Bluetooth
  --------------------------------------------------------------
  No separate remote at all — your phone IS the remote. A Bluetooth
  module (HC-05 / HC-06) on the car talks to the free "Dabble" app,
  whose on-screen GamePad sends the direction. Same motor functions again;
  only the input changed one last time.

  Setup:
    1) Install "Dabble" from the app store on your phone.
    2) Library: "Dabble" (Manage Libraries -> search "Dabble").
    3) In the app: connect to the HC-05, open the GamePad module.

  Motors: 4 motors, ONE L298N. Both LEFT motors share channel A, both RIGHT
  motors share channel B - so the code controls just two sides (as always).

  Wiring (the car):
    HC-05:  VCC -> 5V   GND -> GND
            HC-05 TX -> Arduino RX (D0)
            HC-05 RX -> Arduino TX (D1)   (cross TX<->RX!)
    L298N ch. A (both LEFT motors):  ENA D5, IN1 D2, IN2 D3
    L298N ch. B (both RIGHT motors): ENB D6, IN3 D4, IN4 D7

  !! IMPORTANT: D0/D1 are also the USB upload pins. UNPLUG the HC-05's
     TX & RX wires while uploading, then reconnect them. (Cleaner option:
     use SoftwareSerial on two free pins so you never have to unplug.)
*/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

// --- L298N motor driver (same pins as Lesson 2) ---
const int ENA = 5, IN1 = 2, IN2 = 3, IN3 = 4, IN4 = 7, ENB = 6;
const int SPEED = 160;

void setup() {
  Dabble.begin(9600);        // talk to the HC-05
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopCar();
}

void loop() {
  Dabble.processInput();     // read what the phone sent

  if      (GamePad.isUpPressed())    forward();
  else if (GamePad.isDownPressed())  backward();
  else if (GamePad.isLeftPressed())  turnLeft();
  else if (GamePad.isRightPressed()) turnRight();
  else                               stopCar();   // finger off -> stop
}

// ---------- the four moves (identical to every other version) ----------
void forward()  { digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW); digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW); analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void backward() { digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void turnRight(){ digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW); digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void turnLeft() { digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW); analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void stopCar()  { analogWrite(ENA,0);analogWrite(ENB,0); digitalWrite(IN1,LOW);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,LOW); }
