/*
  Robotika – Lesson 3 · LEVEL 0: Finish the self-driving car
  ----------------------------------------------------------
  Last lesson our car only STOPPED in front of an obstacle.
  A real self-driving car should get itself OUT of trouble:
      clear ahead  -> drive forward
      blocked      -> back up a little, turn away, then carry on

  Same 4-motor car + L298N H-bridge + front HC-SR04 as Lesson 2.
  We only add three new moves (backward, turnLeft, turnRight) and
  a smarter decision. The wiring is EXACTLY the same as Lesson 2.

  HOW THE MOTORS ARE WIRED (important):
    The car has 4 motors but only ONE L298N, which has TWO channels.
    The two LEFT motors are wired together into channel A, and the two
    RIGHT motors are wired together into channel B. So in code there are
    only two "sides" to control (left, right) - exactly what the functions
    below do. Two motors share one channel, so they pull more current:
    use a proper motor battery, not the Arduino's 5V.
    (Tip: if the two motors on a side fight each other, flip the two wires
     of ONE of them so both spin the same way.)

  Wiring (unchanged from Lesson 2):
    HC-SR04:  Trig -> D9   Echo -> D10   VCC -> 5V   GND -> GND
    L298N channel A (both LEFT motors -> OUT1/OUT2):  ENA -> D5 (PWM)  IN1 -> D2  IN2 -> D3
    L298N channel B (both RIGHT motors -> OUT3/OUT4): ENB -> D6 (PWM)  IN3 -> D4  IN4 -> D7
    L298N power: motor battery + -> 12V   battery - -> GND
                 L298N GND <-> Arduino GND   (MUST share ground)
*/

// --- HC-SR04 (front) ---
const int trigPin = 9;
const int echoPin = 10;

// --- L298N motor driver ---
const int ENA = 5;   // left speed  (PWM)
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 7;
const int ENB = 6;   // right speed (PWM)

const int SPEED   = 160;  // 0..255 – driving speed
const int STOP_CM = 15;   // "something is in the way" distance (cm)

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  stopCar();
  delay(2000);          // wait 2s before we start driving
}

void loop() {
  int distanceCm = getDistanceCm();
  Serial.print("Distance: "); Serial.print(distanceCm); Serial.println(" cm");

  // ---- the decision ----
  if (distanceCm <= STOP_CM) {
    // blocked: get out of trouble
    stopCar();      delay(200);
    backward();     delay(400);   // reverse a little
    stopCar();      delay(200);
    turnRight();    delay(400);   // spin away from the obstacle
    stopCar();      delay(200);
    // next loop() checks again and drives on if it's now clear
  } else {
    forward();      // path is clear -> keep driving
  }
}

// ---------- the four moves ----------
// Each "side" here = BOTH motors on that side (they share one L298N channel).
void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);   // left side forward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);   // right side forward
  analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);   // left side back
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);   // right side back
  analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);   // left side forward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);  // right side back -> spins right
  analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);  // left side back
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);   // right side forward -> spins left
  analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
}

void stopCar() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// ---------- the sensor ----------
int getDistanceCm() {
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 25000);  // timeout so we never get stuck
  if (duration == 0) return 999;                  // no echo = nothing close = clear
  return duration * 0.0343 / 2;
}
