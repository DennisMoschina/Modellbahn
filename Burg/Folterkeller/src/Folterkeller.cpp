#include <Arduino.h>
#include "Modellbahn.h"

#define ADDRESS 1

//Pin für die LED
#define LEDPIN 11
//Anzahl der Angeschlossenen LEDs
#define NUMPIXELS 4

#define FIRE_PIN 8
#define LIGHT_PIN 9

// Zeit, die die Show dauern soll
#define SHOW_DURATION 10


SoundDioramaSlave dioramaSlave(ADDRESS);
NeoPixel led(NUMPIXELS, LEDPIN);


void startShow() {
  digitalWrite(FIRE_PIN, HIGH);
  analogWrite(LIGHT_PIN, 50);

  led.torch(SHOW_DURATION);

  led.off();
  digitalWrite(FIRE_PIN, LOW);
  digitalWrite(LIGHT_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FIRE_PIN, OUTPUT);

  dioramaSlave.begin();
}

void loop() {
  if (dioramaSlave.shouldPerform()) {
    startShow();
    dioramaSlave.performed();
  }
}
