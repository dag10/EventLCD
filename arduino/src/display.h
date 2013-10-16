/* Display controller. */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <LiquidCrystal595.h>
#include <SPI.h>
#include <Ethernet.h>

const int width = 20;
const int height = 4;

typedef enum {
  SCREEN_NETWORK,
} Screen;

typedef enum {
  DISCONNECTED,
  CONNECTED,
  CONNECTING,
} NetworkStatus;

// TODO Make enum of updatable fields, and a bool array of what needs updating
//      so as to allow updates of parts of the screen.

class Display {
  public:
    Display(uint8_t data, uint8_t clock, uint8_t latch);

    void update(float elapsed);

    Screen getScreen();
    void setScreen(Screen screen);

    void setMAC(uint8_t mac[]);
    void setIP(const uint8_t a, const uint8_t b, const uint8_t c, const uint8_t d);
    void setNetworkStatus(NetworkStatus status);
    
  private:
    LiquidCrystal595 *lcd;
    Screen screen;
    bool needs_update;

    // Network screen
    uint8_t mac[6];
    uint8_t ip[4];
    NetworkStatus network_status;
};

#endif

