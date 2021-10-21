#include <Arduino.h>
#include <Wire.h>

#include "Modellbahn.h"

#include "DFRobotDFPlayerMini.h"
#include "SoundDioramaMaster.h"

#define NUM_BUTTON 5

// (Pin des Tasters (0-7), Pin des Lichts (gerade Zahlen 34-48),
// (optional: Zeit, die der Taster deaktiviert ist in Sekunden))
Button buttonArr[NUM_BUTTON] = {Button(7, 34, 90),
                                Button(6, 36, 90),
                                Button(5, 38, 90),
                                Button(4, 40, 90),
                                Button(3, 42, 90)};
// Addressen müssen bei Master und Slave manuell eingestellt werden!!!
byte slaveAddr[NUM_BUTTON] = {0, 1, 2, 3, 4};

DFRobotDFPlayerMini soundModule;
SoundDioramaMaster dioramaMaster(&soundModule);


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);  // Serial for the Sound module

  dioramaMaster.begin();

  // Versuche mit dem SoundModul zu kommunizieren (30 Versuche).
  for (int i = 0; i < 30 && !soundModule.begin(Serial1); i++) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    delay(500);
  }

  // Lautstaerke von 0 bis 30
  soundModule.volume(30);

  Serial.println("about to init butons");

  // füge jedem Button die Funktion sendStart mit der jeweiligen Addresse hinzu
  for(int i = 0; i < NUM_BUTTON; i++) {
    buttonArr[i].setCallback(CallLambda([i] () {
      Serial.print("Pressed ");
      Serial.println(i);
      dioramaMaster.sendStart(slaveAddr[i]);
      return 0;
    }));
    ButtonManager::addButton(&buttonArr[i]);
    Serial.println("added button to manager");
    dioramaMaster.addSlave(slaveAddr[i]);
    Serial.println("added Slave");
  }
  Serial.println("finished setup");
}


void loop() {
  dioramaMaster.handleSound();
  // überprüfe die Buttons
  ButtonManager::handleButtons();
  delay(20);
}
