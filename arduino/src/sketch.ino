/* Event LCD Arduino code.
 * Copyright 2013 Drew Gottlieb
 */

#include "backlight.h"
#include "display.h"
#include <SPI.h>
#include <Ethernet.h>
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
Backlight* backlight;
Display* display;

// Init
void setup() {
  // Initialize display
  backlight = new Backlight(pin_backlight);
  display = new Display(pin_lcd_data, pin_lcd_clock, pin_lcd_latch);
  display->setScreen(SCREEN_NETWORK);
  display->setMAC(mac);
  display->setNetworkStatus(CONNECTING);

  // Fade up backlight
  while (backlight->getRealBrightness() < 100) {
    float elapsed = delay_interval / 1000.f;
    display->update(elapsed);
    backlight->update(elapsed);
    delay(delay_interval);
  }

  // Connect to network
  bool assignedIP = Ethernet.begin(mac);

  // Check if we connected
  if (assignedIP) {
    display->setIP(Ethernet.localIP());
    display->setNetworkStatus(CONNECTED);
  } else {
    display->setNetworkStatus(DISCONNECTED);
  }
}

// Loop
void loop() {
  float elapsed = delay_interval / 1000.f;
  display->update(elapsed);
  backlight->update(elapsed);
  delay(delay_interval);
}

