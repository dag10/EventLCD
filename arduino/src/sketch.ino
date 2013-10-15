/* Event LCD Arduino code.
 * Copyright 2013 Drew Gottlieb
 */

#include "backlight.h"

// Pins
int pin_backlight = 9;

// Controllers
Backlight* backlight;

// Init
void setup() {
  backlight = new Backlight(pin_backlight);
}

// Loop
void loop() {
  float brightness = backlight->getRealBrightness();

  if (brightness == 100) {
    backlight->setBrightness(20);
    delay(1500);
  } else if (brightness == 20) {
    backlight->setBrightness(100);
    delay(1500);
  }
  
  backlight->update(0.01f);
  delay(10);
}

