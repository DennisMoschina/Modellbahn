#include <Arduino.h>
#include <Modellbahn.h>

#define ADDRESS 2
#define TRAIN_PIN 12
#define LED_PIN 11

// Zeit die die Lampen beim Flackern aus sind
#define FLICKERING_TIME 5
// Helligkeit der Beleuchtung
#define LIGHTING_BRIGHTNESS 50

SoundDioramaSlave dioramaSlave(ADDRESS);

void startTrain() {
  digitalWrite(TRAIN_PIN, HIGH);
  delay(5000);
  digitalWrite(TRAIN_PIN, LOW);
}

void setup() {
  pinMode(TRAIN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  dioramaSlave.begin();

  randomSeed(analogRead(A0));

  analogWrite(LED_PIN, LIGHTING_BRIGHTNESS);
}

void loop() {
  // lasse die Beleuchtung zufällig flackern
  if (random(1, 10000) == 1) {
    analogWrite(LED_PIN, 0);
    delay(FLICKERING_TIME);
    analogWrite(LED_PIN, LIGHTING_BRIGHTNESS);
  }

  if (dioramaSlave.shouldPerform()) {
    startTrain();
    dioramaSlave.performed();
  }
}
