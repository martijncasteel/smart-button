#include "smart-button.h"

#include "source/elgato-lights.cpp"
#include "source/hue-lights.cpp"
#include "source/hue-groups.cpp"

// button order from left to right
int buttons[3] = { 14, 12, 13 };
int state[3] = { HIGH, HIGH, HIGH };
int value;

// Available lights to interact with
Light *tvLight;
Light *tableLight;
Light *brorLight;

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

  // retrieve local ip-address and get user
  address_t bridge = HueLight::connect(HUE_BRIDGE, LED_BUILTIN, buttons[0]); // move struct to shared place?

  tvLight = new HueGroup(bridge, 7); // 4 with staande lamp
  tableLight = new HueGroup(bridge, 3);
  brorLight = new HueGroup(bridge, 2);

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
      tvLight->toggle();
    }
  }

  value = digitalRead(buttons[1]);

  if ( value != state[1] ) {
    state[1] = value;

    // Increase brightness of key light
    if ( state[1] == LOW ) {
      Serial.println("button 1 pressed");
      tableLight->toggle();
    }
  }

  // Decrease brightness of key light
  value = digitalRead(buttons[2]);
  if ( value != state[2] ) {
    state[2] = value;

    if ( state[2] == LOW ) {
      Serial.println("button 2 pressed");
      brorLight->toggle();
    }
  }

  delay(50);
}
