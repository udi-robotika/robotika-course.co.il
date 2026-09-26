/*
  Robotika – Lesson 3 · LEVEL 2b: RF433 remote (the CAR / RECEIVER)
  ----------------------------------------------------------------
  The car listens for radio codes from level2_rf_transmitter.ino and
  drives accordingly. Same motor functions as always — only the input
  changed again (now it arrives by radio).

  !! ONE WIRING CHANGE !!
  rc-switch must read the radio on an INTERRUPT pin. On the Uno those are
  D2 and D3 — but our motor wire IN1 was on D2. So we MOVE IN1 from D2 to D8,
  and put the RF receiver's DATA on D2. Everything else stays the same.

  Library: "rc-switch"

  Motors: 4 motors, ONE L298N. Both LEFT motors share channel A, both RIGHT
  motors share channel B - so the code controls just two sides (as always).

  Wiring (the car):
    RF receiver (XY-MK-5V): DATA -> D2   VCC -> 5V   GND -> GND
    L298N ch. A (both LEFT motors):  ENA D5, IN1 D8 (moved!), IN2 D3
    L298N ch. B (both RIGHT motors): ENB D6, IN3 D4, IN4 D7
*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

// --- L298N motor driver (IN1 moved to D8 to free the interrupt pin D2) ---
const int ENA = 5, IN1 = 8, IN2 = 3, IN3 = 4, IN4 = 7, ENB = 6;
const int SPEED = 160;

// same codes the transmitter agreed on
const long CODE_STOP = 10, CODE_UP = 11, CODE_DOWN = 12, CODE_LEFT = 13, CODE_RIGHT = 14;

unsigned long lastCmd = 0;
const int HOLD_MS = 300;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);   // interrupt 0 = pin D2
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopCar();
}

void loop() {
  if (mySwitch.available()) {
    long code = mySwitch.getReceivedValue();

    if      (code == CODE_UP)    forward();
    else if (code == CODE_DOWN)  backward();
    else if (code == CODE_LEFT)  turnLeft();
    else if (code == CODE_RIGHT) turnRight();
    else if (code == CODE_STOP)  stopCar();

    lastCmd = millis();
    mySwitch.resetAvailable();
  }

  // radio dropped out? stop, so the car never runs away
  if (millis() - lastCmd > HOLD_MS) stopCar();
}

// ---------- the four moves (identical, just IN1 is now D8) ----------
void forward()  { digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW); digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW); analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void backward() { digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void turnRight(){ digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW); digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void turnLeft() { digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW); analogWrite(ENA,SPEED);analogWrite(ENB,SPEED); }
void stopCar()  { analogWrite(ENA,0);analogWrite(ENB,0); digitalWrite(IN1,LOW);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,LOW); }
