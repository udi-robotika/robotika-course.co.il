/*
  Robotika – Lesson 3 · LEVEL 1a: Learn your IR remote's button codes
  -------------------------------------------------------------------
  Before we can DRIVE with the remote, we need to know what number each
  button sends. Every remote is a little different, so we read them once
  and write the numbers down.

  Upload this, open Tools -> Serial Monitor (9600 baud), point the small
  remote at the receiver and press each button. Copy the HEX code shown
  for UP / DOWN / LEFT / RIGHT / OK into the next sketch.

  Library: "IRremote" by shirriff / Armin Joachimsmeyer
           (Sketch -> Include Library -> Manage Libraries -> search "IRremote")

  Wiring (IR receiver module, e.g. VS1838 / HX-M121):
    OUT/S -> D8      VCC/+ -> 5V      GND/- -> GND
*/

#include <IRremote.h>

const int IR_RECEIVE_PIN = 8;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);
  Serial.println("Point the remote and press buttons...");
}

void loop() {
  if (IrReceiver.decode()) {
    // the "command" byte is the part that changes per button
    Serial.print("command (HEX): 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume();   // ready for the next button
  }
}
