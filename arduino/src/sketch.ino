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
const int delay_interval = 10; // milliseconds between ticks in sleep()
uint8_t mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0xBE, 0x94}; // MAC of board
const float refresh_interval = 20; // seconds between each refresh TODO make 20
const float reconnect_interval = 10; // seconds between reconnection attempts
const int max_http_retries = 3; // max HTTP retry attempts before iface retry
const char *domain = "rancor.csh.rit.edu"; // domain of website
const uint16_t port = 5000; // port of website
const char *location = "lounge"; // location to search for in calendar
const char *location_full = "Lounge"; // location to display on screen
const uint8_t lcd_width = 20; // characters in each LCD row
const uint8_t lcd_height = 4; // rows in LCD display

// Controllers
Backlight backlight(pin_backlight);
Display display(
    pin_lcd_data, pin_lcd_clock, pin_lcd_latch, lcd_width, lcd_height);
Request request;

// Functions
void setup();
void loop();
bool maintainLease();
void updateDisplay();
void sleep(float seconds);

// Variables
bool connected = false;
uint8_t retries = 0;
char *url;

// Init
void setup() {
  // Initialize display
  backlight.setBrightness(100);
  display.setScreen(SCREEN_NETWORK);
  display.setMAC(mac);
  display.setNetworkStatus(CONNECTING);

  // Wait for brightness to fade up
  while (backlight.getRealBrightness() < backlight.getBrightness())
    sleep(0.1);

  // Create URL path
  const char *url_format = "/location/%s/events";
  url = new char[strlen(url_format) - 2 + strlen(location) + 1];
  sprintf(url, url_format, location);
}

// Loop
void loop() {
  // Make sure we're still connected to the network
  while (!connected) {
    connected = Ethernet.begin(mac);
    if (!connected) {
      display.setScreen(SCREEN_NETWORK);
      display.setNetworkStatus(DHCP_FAILED);
      backlight.flash(2);
      sleep(30);
      display.setNetworkStatus(CONNECTING);
      sleep(0.1);
    }
  }

  // We're connected now, so update display
  display.setNetworkStatus(CONNECTED);
  display.setIP(Ethernet.localIP());
  sleep(1);

  // Maintain DHCP lease to make sure we're still totally connected
  do {
    request.setup(domain, url, port);
    display.setRequest(&request);

    // Step through the HTTP request
    while (request.step())
      if (display.getScreen() == SCREEN_NETWORK)
        updateDisplay();
    
    // HTTP request failed, so display error message and wait a moment
    if (request.failed()) {
      display.setScreen(SCREEN_NETWORK);
      backlight.flash(2);
      sleep(reconnect_interval);

      // If it tries reconnecting to HTTP too many times, try reconnecting
      // on the interface itself.
      if (++retries >= max_http_retries)
        break;

      continue;
    }
    retries = 0;
    char *response = (char*)request.getData();

    // Parse requested backlight brightness from response body
    for (char *c = response; *c; c++) {
      if (*c == '\n') {
        *c = 0;
        backlight.setBrightness(atoi(response) / 255.f * 100.f);
        response = c + 1;
        break;
      }
    }

    // Show HTTP response body on screen then wait a bit before refreshing
    display.setScreen(SCREEN_TEXT);
    display.setContent(response);
    sleep(refresh_interval);
  } while (maintainLease());

  // Disconnected (failed to maintain), so wait a bit before reconnecting
  display.setScreen(SCREEN_NETWORK);
  display.setNetworkStatus(DISCONNECTED);
  backlight.flash(2);
  sleep(reconnect_interval);
  display.setNetworkStatus(RECONNECTING);
  sleep(0.1);
  connected = false;
}

// Maintains the DHCP lease, returns true if connection still good
bool maintainLease() {
  uint8_t maintain_status = Ethernet.maintain();
  return maintain_status == 0 || maintain_status % 2 == 0;
}

// Update display
void updateDisplay() {
  static unsigned long last_update = millis() - 1;
  float elapsed = (millis() - last_update) / 1000.f;
  display.update(elapsed);
  backlight.update(elapsed);
  last_update = millis();
}

// Pause the program, but continue updating the display
void sleep(float seconds) {
  unsigned long target = millis() + (seconds * 1000);
  while (millis() < target) {
    updateDisplay();
    delay(delay_interval);
  }
}

