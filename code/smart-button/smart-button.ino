#include "smart-button.h"

#include "elgato-lights.cpp"
// #include "hue-lights.cpp"

// button order from left to right
int buttons[3] = { 14, 12, 13 };
int state[3] = { HIGH, HIGH, HIGH };
int value = HIGH;

// Available lights to interact with
Light *keyLight;
Light *studyLight;

void setup() {
  Serial.begin(9600);

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

  // retrieve local ip-address and get user      
  // const char * address = HueLight::connect();
  // studyLight = new HueLight(address, HUE_USER, 4);
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

  value = digitalRead(buttons[1]);

  if ( value != state[1] ) {
    state[1] = value;

    // Increase brightness of key light
    if ( state[1] == LOW ) {
      Serial.println("button 1 pressed");
      keyLight->set(keyLight->brightness + 10);
    }
  }

  // Decrease brightness of key light
  value = digitalRead(buttons[2]);
  if ( value != state[2] ) {
    state[2] = value;

    if ( state[2] == LOW ) {
      Serial.println("button 2 pressed");
      keyLight->set(keyLight->brightness - 10);
    }
  }
}
