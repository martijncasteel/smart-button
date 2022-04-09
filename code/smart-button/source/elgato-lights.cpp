#include "../smart-button.h"

/**
 * @file elgato-lights.cpp
 * @date 2022-04-05
 * 
 * @brief Lights class for elgato key light, light can be turned on and off, add
 * the brightness can be adjusted.
 */

class ElgatoLight : public Light {
  public:
    ElgatoLight(char * url) : Light() {
      this->url = url;

      // retrieve state of light
      this->getState();
    }

    bool toggle() {
      JSONVar json;
      json["lights"][0]["on"] = this->state ? 0 : 1;

      // put new values to keylight
      http.begin(client, this->url);
      http.addHeader("Content-Type", "application/json");
      int code = http.PUT(JSON.stringify(json)); 

      if ( code != HTTP_CODE_OK ) {
        http.end();
        return false;
      }

      http.end();
      this->state = !this->state;

      return true;  
    }

    bool set(int brightness) {
      if ( brightness < 3 ) {
        brightness = 3;
      } else if ( brightness > 100 ) {
        brightness = 100;
      }

      if ( this->brightness == brightness ) {
        return false;
      }

      JSONVar json;
      json["lights"][0]["brightness"] = brightness;

      // put new values to keylight
      http.begin(client, this->url);
      http.addHeader("Content-Type", "application/json");
      int code = http.PUT(JSON.stringify(json)); 

      if ( code != HTTP_CODE_OK ) {
        http.end();
        return false;
      }

      http.end();
      this->brightness = brightness;

      return true;  
    }

  private:
    char * url;

    bool getState() {
      http.begin(client, this->url);
      int code = http.GET();

      if ( code != HTTP_CODE_OK ) {
        http.end();
        return false;
      }

      String payload = http.getString();
      http.end();

      // {"numberOfLights":1,"lights":[{"on":1,"brightness":33,"temperature":326}]}
      JSONVar json = JSON.parse(payload);

      if (JSON.typeof(json) == "undefined") {
        return false;
      }

      this->state = json["lights"][0]["on"];
      this->brightness = json["lights"][0]["brightness"];

      Serial.print("Elgato Keylight state:");
      Serial.print(this->state);
      Serial.print(" brightness:");
      Serial.print(this->brightness);

      return true;
    }
};