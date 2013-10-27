/* Backlight controller. */

#include "backlight.h"
#include <Arduino.h>

Backlight::Backlight(int pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT);

  animate_speed = 10;
  animate = true;

  sleep_remaining = 0;

  brightness = 0;
  brightness_target = 100;

  flash_min_brightness = 50;
  flash_max_brightness = 100;
  flash_animate_speed = 150;
  flashes_remaining = 0;
}

void Backlight::update(float elapsed) {
  if (sleep_remaining > 0) {
    sleep_remaining -= elapsed;
    if (sleep_remaining > 0)
      return;
    sleep_remaining = 0;
  }

  // How much brightness should change right now
  float speed = flashes_remaining > 0 ? flash_animate_speed : animate_speed;
  float delta = speed * elapsed;

  // If flashing, manage animation
  if (flashes_remaining > 0) {
    if (flashes_remaining % 2 == 0 &&
        brightness <= flash_min_brightness) {
      brightness_target = flash_max_brightness;
      flashes_remaining--;
    } else if (flashes_remaining % 2 == 1 &&
        brightness >= flash_max_brightness) {
      brightness_target = flash_min_brightness;
      flashes_remaining--;
    }

    if (flashes_remaining == 0)
      brightness_target = preflash_brightness;
  }

  if (animate) {
    if (abs(brightness - brightness_target) < delta) {
      brightness = brightness_target;
    } else {
      if (brightness < brightness_target)
        brightness += delta;
      else
        brightness -= delta;
    }
  } else {
    brightness = brightness_target;
  }

  // Subtle pulsing heartbeat
  float seconds = millis() / 1000.f;
  float degrees = seconds * 90.f;
  float heartbeat = (sin(degrees * 3.14156f / 180) + 1.f) / 2.f;

  analogWrite(pin, 255.f / 100.f * (brightness * (1.f - (0.3f * heartbeat))));
}

bool Backlight::animationEnabled() {
  return animate;
}

void Backlight::setAnimation(bool enabled) {
  animate = enabled;
}

void Backlight::setBrightness(float brightness) {
  this->brightness_target = brightness;
  flashes_remaining = 0;
}

float Backlight::getBrightness() {
  return brightness_target;
}

float Backlight::getRealBrightness() {
  return brightness;
}

void Backlight::sleep(float seconds) {
  sleep_remaining += seconds;
}

void Backlight::flash(int times) {
  if (flashes_remaining == 0) {
    preflash_brightness = brightness_target;
    brightness_target = flash_min_brightness;
  }

  flashes_remaining += times * 2;
}

