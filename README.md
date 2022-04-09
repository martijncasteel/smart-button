# Smart-button
Simple shield for Nodemcu v3 with three cherrymx buttons. The different buttons can control Elgato lights or Hue Lights. This project came forward out the necesity of having a button to control a Elgato keylight without having to use their app.

## Usage
Controlling a Elgato keylight can be done with the following;
```c++
Light * keyLight = new ElgatoLight("http://elgato-key-light-air-1857.local:9123/elgato/lights");
keyLight->toggle();
```

Controlling a Hue light is a little bit more complex, the device needs to create an account on the Hue bridge. When starting the device it will connect to the wifi configured in `secret.h`. When the light is blinking you'll have to press the button on the bridge. When pressed you'll have 30 seconds to press the first button on our shield.
```c++

```

## Shield
