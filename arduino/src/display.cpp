/* Display controller. */

#include "display.h"

Display::Display(uint8_t data, uint8_t clock, uint8_t latch) {
  lcd = new LiquidCrystal595(data, latch, clock);
  lcd->begin(width, height);
  lcd->clear();
  needs_update = true;
}

void Display::update(float elapsed) {
  switch (screen) {
    case SCREEN_NETWORK:
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

        if (network_status == CONNECTED) {
          lcd->setCursor(0, 1);
          lcd->print("IP: ");
          lcd->print("todo");
          lcd->setCursor(0, 2);
          lcd->print("MAC: todo");
        }
      }
      break;
  }

  needs_update = false;
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

void Display::setIP(const uint8_t a, const uint8_t b, const uint8_t c, const uint8_t d) {
  ip[0] = a;
  ip[1] = b;
  ip[2] = c;
  ip[3] = d;
  if (screen == SCREEN_NETWORK)
    needs_update = true;
}

void Display::setNetworkStatus(NetworkStatus status) {
  network_status = status;
  if (screen == SCREEN_NETWORK)
    needs_update = true;
}

