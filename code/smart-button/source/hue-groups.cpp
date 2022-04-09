#include "../smart-button.h"
#include "helpers/storage.cpp"

/**
 * @file hue-lights.cpp
 * @date 2022-04-09
 * 
 * @brief Lights class for hue lights. Device has to create an account and
 * stores it in EEPROM, the account can be used for multiple lights.
 */

class HueGroup : public Light {
  public:
    HueGroup(address_t address, int id) : Light() {
      address.id = id;
      address.element = "groups";

      this->address = address;
      this->getState();
    }

    bool toggle() {
      JSONVar json;
      json["on"] = !this->state;

      http.begin(client, this->address.toString("action"));
      int code = http.PUT(JSON.stringify(json));
      
      if ( code != HTTP_CODE_OK ) {
        Serial.print("Error on retrieving state ");
        Serial.print(this->address.toString("state"));
        Serial.println(http.errorToString(code).c_str());
        http.end();
        return false;
      }

      http.end();
      this->state = !this->state;

      return true;  
    }

    bool set(int brightness) {
      return false;
    }

  private:
    bool getState() {
      http.begin(client, this->address.toString());
      int code = http.GET();

      if ( code != HTTP_CODE_OK ) { 
        Serial.print("Error retrieving Hue state: ");
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

      this->state = json["state"]["all_on"];
      this->brightness = json["action"]["bri"];

      Serial.print("Huegroup state:");
      Serial.print(this->state);
      Serial.print(" brightness:");
      Serial.print(this->brightness);

      return true;
    }

    address_t address;
};

