/* Display controller. */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <arduino.h>
#include <LiquidCrystal595.h>

// LCD width and height
const int width = 20;
const int height = 4;

// Possible screens to show
typedef enum {
  SCREEN_NETWORK,
} Screen;

// Possible network statuses
typedef enum {
  DISCONNECTED,
  CONNECTED,
  CONNECTING,
} NetworkStatus;

class Display {
  public:
    Display(uint8_t data, uint8_t clock, uint8_t latch);

    void update(float elapsed);

    Screen getScreen();
    void setScreen(Screen screen);

    void setMAC(uint8_t mac[]);
    void setIP(uint32_t ip);
    void setNetworkStatus(NetworkStatus status);
    
  private:
    LiquidCrystal595 *lcd;
    Screen screen;
    bool needs_update;

    // Network data
    uint8_t mac[6];
    uint32_t ip;
    NetworkStatus network_status;
};

#endif

