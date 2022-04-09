#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

#include "secrets.h"

#ifndef LIGHT_H_
#define LIGHT_H_
#include "source/light.h"
#endif

#ifndef SMART_BUTTON_H_
#define SMART_BUTTON_H_
inline WiFiClient client;
inline HTTPClient http;
#endif
