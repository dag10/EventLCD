/* Event LCD Arduino code.
 * Copyright 2013 Drew Gottlieb
 */

#include "backlight.h"
#include "display.h"
#include "request.h"
#include <inttypes.h>

// Pins
uint8_t pin_backlight = 9;
uint8_t pin_lcd_data = 7;
uint8_t pin_lcd_clock = 6;
uint8_t pin_lcd_latch = 5;

// Settings
const int delay_interval = 10;
uint8_t mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0xBE, 0x94};

// Controllers
Backlight backlight(pin_backlight);
Display display(pin_lcd_data, pin_lcd_clock, pin_lcd_latch);
Request request;

// Functions
void setup();
void loop();
void updateDisplay();
void sleep(float seconds);

// Init
void setup() {
  // Initialize display
  backlight.setBrightness(100);
  display.setScreen(SCREEN_NETWORK);
  display.setMAC(mac);
  display.setNetworkStatus(CONNECTING);

  // Wait for brightness to fade up
  while (backlight.getRealBrightness() < backlight.getBrightness()) {
    updateDisplay();
    delay(delay_interval);
  }

  // Connect to network
  bool assignedIP = Ethernet.begin(mac);

  // Check if we connected
  if (assignedIP) {
    display.setIP(Ethernet.localIP());
    display.setNetworkStatus(CONNECTED);
    sleep(0.2);
    backlight.flash(1);
  } else {
    display.setNetworkStatus(DISCONNECTED);
    sleep(0.2);
    backlight.flash(2);
    return;
  }

  // Make single request
  request.setup("rancor.csh.rit.edu", "/location/lounge/events/3", 5000);
  display.setRequest(&request);

  sleep(1);
  while (request.step()) {
    updateDisplay();
    sleep(0.1);
  }
  
  if (request.failed())
    return;

  display.setScreen(SCREEN_TEXT);
  display.setContent(request.getData());
}

// Loop
void loop() {
  sleep(0.1);
}

// Update display
void updateDisplay() {
  static int last_update = millis() - 1;
  float elapsed = (millis() - last_update) / 1000.f;
  display.update(elapsed);
  backlight.update(elapsed);
  last_update = millis();
}

// Pause the program, but continue updating the display
void sleep(float seconds) {
  for (float s = 0; s < seconds; s += delay_interval / 1000.f) {
    updateDisplay();
    delay(delay_interval);
  }
}

