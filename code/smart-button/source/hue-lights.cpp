#include "../smart-button.h"
#include "helpers/storage.cpp"

/**
 * @file hue-lights.cpp
 * @date 2022-04-09
 * 
 * @brief Lights class for hue lights. Device has to create an account and
 * stores it in EEPROM, the account can be used for multiple lights.
 */

struct address_t {
  String address;
  String user;
  int id = 0;

  String toString(String action = "") {
    return String("http://") + address + "/api/" + user + "/lights/" + String(id) + "/" + action;
  }
};


class HueLight : public Light {
  public:
    HueLight(address_t address, int id) : Light() {
      address.id = id;
      this->address = address;

      // retrieve state of light
      this->getState();
    }

    // find ip address, create user and store in eeprom, return full url
    static address_t connect(char * address, int led, int button) {
      String user = Storage::read_string_from_eeprom(0);

      if ( user == NULL ) {

        Serial.println("Push button on Hue bridge and press SW1 to continue...");

        // blink light if bridge button should be pressed
        while( digitalRead(button) != LOW ) {
          digitalWrite(LED_BUILTIN, LOW);
          delay(200);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(200);
        }

        JSONVar json;
        json["devicetype"] = "smart-button#" + WiFi.macAddress();

        // create user
        http.begin(client, "http://" + String(address) + "/api");
        int code = http.POST(JSON.stringify(json));

        if ( code != HTTP_CODE_OK ) {
          Serial.print("Error on creating Hue user: ");
          Serial.println(http.errorToString(code).c_str());
          http.end();
          
          Serial.println("Reset to try again...");
          while(1){
            yield();
          }
        }

        String payload = http.getString();
        http.end();

        json = JSON.parse(payload);

        if ( payload.indexOf("error") > 0 ) {
          Serial.print("Error: ");
          Serial.println(json[0]["error"]["description"]);
          Serial.println("Reset to try again...");
          while(1){
            yield();
          }
        }

        // if structure not found, hard crash will occur
        // user = json[0]["success"]["username"]; TODO fix this
        Serial.println(user);

        // Storage::write_string_to_eeprom(0, user);
      }

      address_t a{ String(address), user, 0 };
      return a;
    }

    bool toggle() {
      Serial.println(this->address.toString("state"));

      JSONVar json;
      json["on"] = !this->state;

      http.begin(client, this->address.toString("state"));
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

      this->state = json["state"]["on"];
      this->brightness = json["state"]["brightness"];

      Serial.print("Huelight state:");
      Serial.print(this->state);
      Serial.print(" brightness:");
      Serial.print(this->brightness);

      return true;
    }

    address_t address;
};

