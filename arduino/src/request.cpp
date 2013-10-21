/* HTTP Request object. */

#include "request.h"
#include <stdlib.h>

void Request::setup(const char *host, const char *path, uint16_t port) {
  this->host = host;
  this->path = path;
  this->port = port;
  buffer_next = 0;
  state = STATE_DISCONNECTED;
  error_message = 0;
  http_status = 0;
  newline_seq = 0;
  capturing_body = false;
}

bool Request::step() {
  switch (state) {
    case STATE_FAILED:
      return false;
    case STATE_DISCONNECTED:
      state = STATE_CONNECTING;
      return true;
    case STATE_CONNECTING:
      return connect();
    case STATE_CONNECTED:
      return downloadResponse();
  }

  return false;
}

bool Request::connect() {
  if (!client.connect(host, port)) {
    state = STATE_FAILED;
    error_message = "Failed to connect.";
    return false;
  }

  get();

  state = STATE_CONNECTED;
  return true;
}

void Request::get() {
  client.print("GET ");
  client.print(path);
  client.println(" HTTP/1.1");

  client.print("Host: ");
  client.println(host);

  client.println("User-Agent: arduino-ethernet");
  client.println("Connection: close");
  
  client.println();
}

bool Request::downloadResponse() {
  // Check for a disconnect
  if (!client.connected()) {
    client.stop();

    if (capturing_body && buffer[0] != 0) {
      state = STATE_SUCCESS;
    } else {
      state = STATE_FAILED;
      error_message = "No body";
    }

    return false;
  }
  
  // If no HTTP status yet, read in the status
  if (http_status == 0) {
    // Skip to first space, read in three digits, parse int
    for (char c = client.read(); c != ' '; c = client.read());
    for (uint8_t i = 0; i < 3; i++)
      buffer[i] = client.read();
    buffer[3] = 0;
    http_status = atoi(buffer);
    buffer[0] = 0;

    if (http_status != HTTP_OK) {
      state = STATE_FAILED;
      error_message = buffer; // I know this is cheap, but I want to conserve
                              // what I can of my 2K memory.
      sprintf(error_message, "Got %d %s", http_status, getResponseStatus());
      return false;
    }
  }

  if (capturing_body) {
    while (client.available()) {
      // Read a byte and put it in the buffer
      char c = client.read();
      buffer[buffer_next] = c;

      // Increment next index in buffer, make sure it doesn't overflow
      if (++buffer_next >= buffer_length - 1) {
        error_message = "Buffer overflow";
        state = STATE_FAILED;
        return false;
      }

      // Make next byte a null terminator unless no more bytes are read
      buffer[buffer_next] = 0;
    }
  } else {
    while (client.available()) {
      char c = client.read();
      // If two \r\n sequences, it's the body start so start capturing
      if (c == '\r' || c == '\n') {
        if (++newline_seq == 4) {
          capturing_body = true;
          break;
        }
      } else {
        newline_seq = 0;
      }
    }
  }

  return true;
}

bool Request::connected() {
  return state == STATE_CONNECTED && client.connected();
}

bool Request::failed() {
  return state == STATE_FAILED;
}

RequestState Request::getState() {
  return state;
}

const char *Request::getStatusString() {
  switch (state) {
    case STATE_DISCONNECTED:
      return "disconnected";
    case STATE_CONNECTING:
      return "connecting";
    case STATE_CONNECTED:
      return "connected";
    case STATE_FAILED:
      return "failed";
    case STATE_SUCCESS:
      return "success";
  }

  return "unknown";
}

const char *Request::getErrorMessage() {
  return error_message;
}

const char *Request::getResponseStatus() {
  switch (http_status) {
    case HTTP_OK:
      return "Ok";
    case HTTP_BAD_REQUEST:
      return "Bad Request";
    case HTTP_FORBIDDEN:
      return "Forbidden";
    case HTTP_NOT_FOUND:
      return "Not Found";
    case HTTP_INTERNAL_ERROR:
      return "Internal Error";
    case HTTP_NOT_IMPLEMENTED:
      return "Not Implemented";
    case HTTP_BAD_GATEWAY:
      return "Bad Gateway";
    case HTTP_SERVICE_UNAVAILABLE:
      return "Unavailable";
    case HTTP_TIMEOUT:
      return "Timeout";
  }

  return "(unknown)";
}

const char *Request::getData() {
  if (state != STATE_SUCCESS || !capturing_body)
    return 0;

  return buffer;
}

