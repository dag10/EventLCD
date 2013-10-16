/* Backlight controller. */

#include "backlight.h"
#include "Arduino.h"

Backlight::Backlight(int pin) {
  this->pin = pin;
  animate_speed = 50;
  brightness = 0;
  brightness_target = 100;
  animate = true;
  pinMode(pin, OUTPUT);
}

void Backlight::update(float elapsed) {
  if (!animate || elapsed == 0) {
    brightness = brightness_target;
  }

  float delta = animate_speed * elapsed;

  if (animate) {
    if (abs(brightness - brightness_target) < delta) {
      brightness = brightness_target;
    } else {
      if (brightness < brightness_target)
        brightness += delta;
      else
        brightness -= delta;
    }
  }

  analogWrite(pin, 255.f / 100.f * brightness);
}

bool Backlight::animationEnabled() {
  return animate;
}

void Backlight::setAnimation(bool enabled) {
  animate = enabled;
}

void Backlight::setBrightness(float brightness) {
  this->brightness_target = brightness;
}

float Backlight::getBrightness() {
  return brightness_target;
}

float Backlight::getRealBrightness() {
  return brightness;
}

