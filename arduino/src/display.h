/* Display controller. */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal595.h>
#include "request.h"

// Possible screens to show
typedef enum {
  SCREEN_NETWORK,
  SCREEN_TEXT,
} Screen;

// Possible network statuses
typedef enum {
  DISCONNECTED,
  DHCP_FAILED,
  CONNECTED,
  CONNECTING,
  RECONNECTING,
} NetworkStatus;

class Display {
  public:
    Display(
        uint8_t data, uint8_t clock, uint8_t latch, uint8_t width,
        uint8_t height);

    void update(float elapsed);

    Screen getScreen();
    void setScreen(Screen screen);

    void setMAC(uint8_t mac[]);
    void setIP(uint32_t ip);
    void setNetworkStatus(NetworkStatus status);
    void setRequest(Request *request);

    void setContent(const char *content);
    
  private:
    void printFirst(const char *str, uint16_t length);

    LiquidCrystal595 *lcd;
    Screen screen;
    bool needs_update;
    uint8_t width;
    uint8_t height;

    // Network data
    uint8_t mac[6];
    uint32_t ip;
    NetworkStatus network_status;
    Request *request;
    RequestState prev_request_state;

    // Text data
    char *content;
};

#endif

