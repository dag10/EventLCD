/* Display controller. */

#include "display.h"

Display::Display(uint8_t data, uint8_t clock, uint8_t latch) {
  lcd = new LiquidCrystal595(data, latch, clock);
  lcd->begin(width, height);
  lcd->clear();

  network_status = DISCONNECTED;
  request = 0;

  needs_update = true;
}

void Display::update(float elapsed) {
  switch (screen) {
    case SCREEN_NETWORK:
      if (request) {
        RequestState new_state = request->getState();
        if (new_state != prev_request_state) {
          needs_update = true;
          prev_request_state = new_state;
        }
      }

      if (needs_update) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("Status: ");
        if (network_status == DISCONNECTED)
          lcd->print("disconnected");
        else if (network_status == CONNECTED)
          lcd->print("connected");
        else if (network_status == CONNECTING)
          lcd->print("connecting");

        // Print MAC address
        lcd->setCursor(0, 1);
        for (int i = 0; i < 6; i++) {
          if (mac[i] < 0x10) lcd->print(0);
          lcd->print(mac[i], HEX);
          if (i < 5) lcd->print(":");
        }

        if (network_status == CONNECTED) {
          // Print IP address
          lcd->setCursor(0, 2);
          for (int i = 0; i < 4; i++) {
            lcd->print(((byte*)&ip)[i], DEC);
            if (i < 3) lcd->print(".");
          }

          // Print HTTP request information
          if (request) {
            lcd->setCursor(0, 3);

            if (request->failed()) {
              lcd->print("!");
              printFirst(request->getErrorMessage(), 20 - 1);
            } else {
              lcd->print("HTTP: ");
              printFirst(request->getStatusString(), 20 - 6);
            }
          }
        }
      }
      break;
  }

  needs_update = false;
}

void Display::printFirst(const char *str, uint16_t length) {
  for (int i = 0; i < length && str[i]; i++)
    lcd->print(str[i]);
}

Screen Display::getScreen() {
  return screen;
}

void Display::setScreen(Screen screen) {
  this->screen = screen;
  needs_update = true;
}

void Display::setMAC(uint8_t mac[]) {
  for (int i = 0; i < 6; i++)
    this->mac[i] = mac[i];
  if (screen == SCREEN_NETWORK)
    needs_update = true;
}

void Display::setIP(uint32_t ip) {
  this->ip = ip;
  if (screen == SCREEN_NETWORK)
    needs_update = true;
}

void Display::setNetworkStatus(NetworkStatus status) {
  network_status = status;
  if (screen == SCREEN_NETWORK)
    needs_update = true;
}

void Display::setRequest(Request *request) {
  this->request = request;
  if (screen == SCREEN_NETWORK)
    needs_update = true;
}

