#include <Arduino.h>
#include <Servo.h>

#include "Modellbahn.h"
// #include "SoundDioramaSlave.h"

// Position in der die Rauchkammer geöffnet ist
#define SERVO_OPEN 90
// Position in der die Rauchkammer geschlossen ist
#define SERVO_CLOSED 0

// Zeit die der Rauchgenerator Rauch produziert
#define HEATING_TIME 9000

// Pin der LEDs die den Blitz erzeugen
#define FLASH_PIN 9
// Anzahl der LEDs die den Blitz erzeugen
#define NUMPIXELS 2

// Pin an den der Servo der Rauchkammer angeschlossen ist
#define SERVO_PIN 8
// Pin an den der Rauchgenerator angeschlossen ist
#define SMOKER_PIN 13
//Pin an den die Alarm LEDs angeschlossen sind
#define ALARM_LED_PIN 10

// Helligkeit der LEDs für die Beleuchtung
#define EXPLOSION_LIGHTING_BRIGHTNESS 10
// Zeit die die Beleuchtung nachleuchtet
#define GLIMMING_TIME 2000

// Intervall in dem die Alarm LEDs ein- und ausgeschaltet werden
#define ALARM_LED_INTERVAL 500

// Adresse des Arduinos für die I2C Kommunikation mit dem Taster Arduino
#define ADDRESS 0

// The sound file the taster arduino should play
#define SOUND_FILE 1
#define SOUND_DELAY 1200

NeoPixel flashLED = NeoPixel(NUMPIXELS, FLASH_PIN);
Servo myServo;

SoundDioramaSlave dioramaSlave(ADDRESS);


// Simuliere die Showsprengung
void explosion() {
  // schalte Beleuchtung der Showsprengung an
  flashLED.fill(CRGB::White);
  flashLED.setBrightness(EXPLOSION_LIGHTING_BRIGHTNESS);

  // Rauch produzieren
  digitalWrite(SMOKER_PIN, HIGH);
  unsigned long startTime = millis();

  // während der Rauchgenerator läuft einen Alarm abspielen
  while (millis() < startTime + HEATING_TIME) {
    digitalWrite(ALARM_LED_PIN, !digitalRead(ALARM_LED_PIN));
    delay(ALARM_LED_INTERVAL);
  }
  digitalWrite(ALARM_LED_PIN, LOW);

  dioramaSlave.playSoundWithID(SOUND_FILE);

  delay(SOUND_DELAY);

  // Rauch aus der Rauchkammer rauslassen
  myServo.write(SERVO_OPEN);

  flashLED.flash();
  digitalWrite(SMOKER_PIN, LOW);

  // für eine Zeit Beleuchtung aktiv lassen
  flashLED.fill(CRGB::White);
  flashLED.setBrightness(EXPLOSION_LIGHTING_BRIGHTNESS);
  delay(GLIMMING_TIME);
  myServo.write(SERVO_CLOSED);
  flashLED.off();
}


void setup() {
  Serial.begin(115200);

  dioramaSlave.begin();

  pinMode(SMOKER_PIN, OUTPUT);
  pinMode(ALARM_LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_CLOSED);
  flashLED.off();
}


void loop() {
  // Überprüfe, ob Explosion stattfinden soll und evtl starten
  bool shouldPerform = dioramaSlave.shouldPerform();
  Serial.print("should perform in loop: ");
  Serial.println(shouldPerform);
  if (shouldPerform) {
    Serial.println("about to perform");
    explosion();
    dioramaSlave.performed();
  }
  delay(100);
}
