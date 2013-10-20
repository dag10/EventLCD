/* HTTP Request object. */

#ifndef _REQUEST_H
#define _REQUEST_H

#include <arduino.h>
#include <Ethernet.h>
#include <inttypes.h>

// Possible request states
typedef enum {
  STATE_DISCONNECTED,
  STATE_CONNECTING,
  STATE_CONNECTED,
  STATE_FAILED,
  STATE_SUCCESS,
} RequestState;

// HTTP response codes
typedef enum {
  HTTP_OK = 200,
  HTTP_BAD_REQUEST = 400,
  HTTP_FORBIDDEN = 403,
  HTTP_NOT_FOUND = 404,
  HTTP_INTERNAL_ERROR = 500,
  HTTP_NOT_IMPLEMENTED = 501,
  HTTP_BAD_GATEWAY = 502,
  HTTP_SERVICE_UNAVAILABLE = 503,
  HTTP_TIMEOUT = 504,
} HTTPResponseCode;

const uint16_t default_port = 80;
const uint16_t buffer_length = 500;

class Request {
  public:
    void setup(const char *host, const char *path, uint16_t port);

    /* Runs next step in connection process.
       Returns true if it should be called again. */
    bool step();

    bool connected();
    bool failed();
    RequestState getState();

    const char *getStatusString();
    const char *getErrorMessage();
    const char *getResponseStatus();

  private:

    /* Returns true if connection was successful and should proceed. */
    bool connect();
    
    /* Sends a GET request. */
    void get();

    bool downloadResponse();

    EthernetClient client;

    char buffer[buffer_length];
    uint16_t buffer_next;
    uint16_t http_status;
    uint16_t newline_seq;
    bool capturing_body;

    const char *host;
    uint16_t port;
    const char *path;

    RequestState state;
    char *error_message;
};

#endif

