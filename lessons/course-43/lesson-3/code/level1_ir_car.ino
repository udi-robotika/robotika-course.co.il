/*
  Robotika – Lesson 3 · LEVEL 1b: Drive the car with an IR remote
  ---------------------------------------------------------------
  THE BIG IDEA: the motor functions are the SAME ones from the
  self-driving car (forward/back/left/right/stop). Last time the SENSOR
  decided which one to call. Now YOU decide, by pressing a button.
  Only the INPUT changed.

  IR = infrared light, just like a TV remote. It needs a clear line of
  sight to the receiver, and only goes one way (remote -> car).

  1) First run level1_ir_read_codes.ino and fill in the 5 codes below.
  2) Then upload this. Point the remote at the car and drive.

  Motors: 4 motors, ONE L298N. Both LEFT motors share channel A, both RIGHT
  motors share channel B - so the code controls just two sides (as always).

  Wiring:
    IR receiver:  OUT -> D8   VCC -> 5V   GND -> GND
    L298N ch. A (both LEFT motors):  ENA D5, IN1 D2, IN2 D3
    L298N ch. B (both RIGHT motors): ENB D6, IN3 D4, IN4 D7
*/

#include <IRremote.h>

const int IR_RECEIVE_PIN = 8;

// --- L298N motor driver (same pins as Lesson 2) ---
const int ENA = 5, IN1 = 2, IN2 = 3, IN3 = 4, IN4 = 7, ENB = 6;
const int SPEED = 160;

// >>> FILL THESE IN from level1_ir_read_codes.ino (your remote's codes) <<<
const uint8_t BTN_UP    = 0x18;
const uint8_t BTN_DOWN  = 0x52;
const uint8_t BTN_LEFT  = 0x08;
const uint8_t BTN_RIGHT = 0x5A;
const uint8_t BTN_OK    = 0x1C;

unsigned long lastCmd = 0;   // when we last heard a button
const int HOLD_MS = 200;     // keep moving this long after a press

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopCar();
}

void loop() {
  if (IrReceiver.decode()) {
    uint8_t cmd = IrReceiver.decodedIRData.command;

    if      (cmd == BTN_UP)    forward();
    else if (cmd == BTN_DOWN)  backward();
    else if (cmd == BTN_LEFT)  turnLeft();
    else if (cmd == BTN_RIGHT) turnRight();
    else if (cmd == BTN_OK)    stopCar();

    lastCmd = millis();
    IrReceiver.resume();   // ready for the next button
  }

  // if no button for a moment, stop (so it doesn't run away)
  if (millis() - lastCmd > HOLD_MS) stopCar();
}

// ---------- the four moves (identical to the self-driving car) ----------
void forward()  { digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW); digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW); analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void backward() { digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void turnRight(){ digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW); digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void turnLeft() { digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW); analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void stopCar()  { analogWrite(ENA,0);analogWrite(ENB,0); digitalWrite(IN1,LOW);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,LOW); }
