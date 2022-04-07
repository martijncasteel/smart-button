#include "smart-button.h"

/**
 * @file hue-lights.cpp
 * @date 2022-04-05
 * 
 * @brief Lights class for hue lights.
 */

class HueLight : public Light {
  public:
    HueLight(const char * address, int id) : Light() {
      this->url = (String(address) + String(id)).c_str();

      // retrieve state of light
      this->getState();
    }

    // find ip address, create user and store in eeprom, return full url
    static const char * connect() {
      return "http://<ip>/api/<user>/lights/";

      // retrieve local ip-adres, WiFiClientSecure
      http.begin(client, "https://discovery.meethue.com/");
      int code = http.GET();

      if ( code != HTTP_CODE_OK ) {
        Serial.print("Error on sending GET request: ");
        Serial.println(http.errorToString(code).c_str());
        http.end();
        return "";
      }

      String payload = http.getString();
      http.end();

      // [{"id":"","internalipaddress":"192.168.178.16","port":443}]
      JSONVar json = JSON.parse(payload);

      if (JSON.typeof(json) == "undefined") {
        return "";
      }

      const char * address = json[0]["internalipaddress"];
      Serial.print("Hue bridge: ");
      Serial.println(address);

      // find or create user, check if user has access, store in eeprom


      // blink light if bridge button should be pressed

      return address;
    }

    bool toggle() {
      JSONVar json;
      json["on"] = !this->state;

      http.begin(client, this->url);
      int code = http.PUT(JSON.stringify(json));
      
      if ( code != HTTP_CODE_OK ) {
        Serial.print("Error on sending GET request: ");
        Serial.println(http.errorToString(code).c_str());
        http.end();
        return false;
      }

      http.end();
      this->state = !this->state;

      return true;  
    }

  private:
    const char * url;

    bool getState() {
      http.begin(client, this->url);
      int code = http.GET();

      Serial.println(this->url);

      if ( code != HTTP_CODE_OK ) { 
        Serial.print("Error on sending GET request: ");
        Serial.println(http.errorToString(code).c_str());
        return false;
      }

      String payload = http.getString();
      http.end();

      JSONVar json = JSON.parse(payload);

      if (JSON.typeof(json) == "undefined") {
        Serial.println("Error: JSON is undefined");
        return false;
      }

      this->state = json["state"]["on"];
      this->brightness = json["state"]["brightness"];

      Serial.print("Huelight state:");
      Serial.print(this->state);
      Serial.print(" brightness:");
      Serial.print(this->brightness);

      return true;
    }
};
