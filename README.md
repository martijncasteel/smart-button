# Smart-button
Simple shield for Nodemcu v3 with three cherrymx buttons. The different buttons can control Elgato lights or Hue Lights. This project came forward out the necesity of having a button to control a Elgato keylight without having to use their app.

![photo-steps](https://user-images.githubusercontent.com/5732642/165465720-b9c3b78a-951a-4225-94ee-248e562a290b.jpg)

## Usage
Start by setting up [source/secrets.h](code/smart-button/secrets.h.example) with your wifi credentials and bridge ip-address if you want to add Hue lights. Adjust your smart-button.ino as you wish to use, below some information on setting up the different lights. 

### Elgato Keylight
Controlling a Elgato keylight can be done with the following;
```c++
// connect with light in setup
Light * keyLight = new ElgatoLight("http://elgato-key-light-air-1857.local:9123/elgato/lights");
keyLight->toggle();
```

### Hue Lights and groups
Controlling a Hue light is a little bit more complex, the device needs to create an account on the Hue bridge. When starting the device it will connect to the wifi configured in `secret.h`. When the light is blinking you'll have to press the button on the bridge. When pressed you'll have 30 seconds to press the first button on our shield.
```c++
  // connect to bridge, create and store user in eeprom
  address_t bridge = HueLight::connect(HUE_BRIDGE, LED_BUILTIN, buttons[0]);

  // control a specific light
  Light * diningLight = new HueLight(bridge, 3);

  // controll all lights within a group
  Light * livingRoom = new HueGroup(bridge, 7);
  livingRoom->toggle();
```

## Shield
![pcb-front](https://user-images.githubusercontent.com/5732642/162588144-cc8d41fe-5cd6-4b2c-abec-196f9c342f36.jpg)

A simple shield made using Kicad to give it a nice aesthetic, you can also use a simple breadboard with simple buttons. The pins use an interal pull-up resistor. When the pin is connected to ground the button is pressed. You can find the source in [pcb](pcb/) and the assembly in [assembly](assembly/) to send to your preferred manufacturer.

## License
This project is licensed under the MIT license. See [LICENSE](LICENSE.md) for more information.