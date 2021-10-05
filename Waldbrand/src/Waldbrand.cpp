#include <Adafruit_NeoPixel.h>

//Auskommentieren um Serielle Komunikation abzuschalten (Ausgabe der Werte auf den Computer)
//#define debug

//PIn für die LED
#define LEDPIN 11

//How many LEDs
#define NUMPIXELS 3

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

//Zeit für das Delay zwischen dem Hochzählen der Helligkeit in der Funtkion waldbrandLicht() in Millisekunden
int delaytime = 2;

void setPixelBrightness(int pixel, unsigned long red, unsigned long green, unsigned long blue, unsigned long brightness) {
  unsigned long redBrightness = red * brightness / 255;
  unsigned long greenBrightness = green * brightness / 255;
  unsigned long blueBrightness = blue * brightness / 255;

  pixels.setPixelColor(pixel, pixels.Color(redBrightness, greenBrightness, blueBrightness));

  pixels.show();

#ifdef debug
Serial.print("Pixel ");
Serial.println(pixel);

Serial.print("Rot: ");
Serial.println(redBrightness);
Serial.print("Gruen: ");
Serial.println(greenBrightness);
Serial.print("Blau: ");
Serial.println(blueBrightness);

Serial.println();
#endif
}


void waldbrandLicht() {
  setPixelBrightness(1, 0, 0, 50, 255);

  int i = 50;
  while(i<255) {

#ifdef debug
Serial.println("---------------------------------------------------");
Serial.print("Hochzählen ");
Serial.println(i);
#endif

    setPixelBrightness(0, 255, 40, 0, i);
    setPixelBrightness(2, 255, 40, 0, 255+50-i);

    i++;

    delay(delaytime);

    if(i>=255) {
      while(i>50) {

#ifdef debug
Serial.println("---------------------------------------------------");
Serial.print("Runterzählen ");
Serial.println(i);
#endif

        setPixelBrightness(0, 255, 40, 0, i);
        setPixelBrightness(2, 255, 40, 0, 255+50-i);

        i = i - 1;

        delay(delaytime);
      }
    }
  }
}



void setup() {
#ifdef debug
Serial.begin(250000);
#endif

  pixels.begin();
}

void loop() {
  waldbrandLicht();
}
