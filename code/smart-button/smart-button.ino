#include "smart-button.h"

#include "source/elgato-lights.cpp"
#include "source/hue-lights.cpp"
#include "source/hue-groups.cpp"

// button order from left to right
int buttons[3] = { 14, 12, 13 };
int state[3] = { HIGH, LOW, LOW };

int value;

// Available lights to interact with
Light *keyLight;
uint step = 5;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  for ( int pin : buttons ) {
    pinMode(pin, INPUT_PULLUP);
  }
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  
  Serial.println("connecting to wifi..");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }

  digitalWrite(LED_BUILTIN, HIGH); 
  Serial.println("connected to wifi!");

  keyLight = new ElgatoLight("http://elgato-key-light-air-1857.local:9123/elgato/lights");
  Serial.println("lights initialized!");
}

void loop() {
  // value for Elgate Key Light state
  value = digitalRead(buttons[0]);
  
  // only toggle once if the button is pressed
  if ( value != state[0] ) {
    state[0] = value;

    if ( state[0] == LOW ) {
      Serial.println("button 0 pressed");
      keyLight->toggle();
    }
  }

  // increase brigtness of keylight
  value = digitalRead(buttons[1]);

  // state is changed
  if ( value != state[1] ) {
    state[1] = value;

    // Increase brightness of key light
    if ( state[1] == HIGH ) {
      Serial.println("button 1 released");

    // started pressing button down
    } else { 
      Serial.println("button 1 pressed");
      keyLight->set(keyLight->brightness + step);
    }

  // button is still pressed
  } else if ( value == LOW ) { 
    Serial.println("button 1 pressed");
    keyLight->set(keyLight->brightness + step);
  }

  // Decrease brightness of key light
  value = digitalRead(buttons[2]);
  
  // state is changed
  if ( value != state[1] ) {
    state[1] = value;

    // Increase brightness of key light
    if ( state[1] == HIGH ) {
      Serial.println("button 1 released");

    // started pressing button down
    } else { 
      Serial.println("button 1 pressed");
      keyLight->set(keyLight->brightness - step);
    }

  // button is still pressed
  } else if ( value == LOW ) { 
    Serial.println("button 1 pressed");
    keyLight->set(keyLight->brightness - step);
  }

  delay(50);
}
