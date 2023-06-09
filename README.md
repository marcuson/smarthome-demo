# How to run demo

1. Start all services:

   ```bash
   docker compose up -d
   ```

1. Services should now run on:
   - HASS: <http://localhost:8123> (to be configured)
   - Node-RED: <http://localhost:1880> (to be configured)
   - Mosquitto: tcp://localhost:1883 (user/pass is hass/hass)

1. Done! You can now try to thinker with your smarthome demo installation!

Notes:

- in the "esp8266" folder you can find a Platformio project which you can build and upload on a
  NodeMCU to test a custom integration with the smarthome setup. You can also find a Postman
  collection to directly interact with your device. However remember to insert your WiFi data
  in the "esp8266/data/wifi.txt" folder before building the firmware.
- in the "examples" folder you can find an example of NodeRED flow to integrate the NodeMCU of
  previous point and Home Assistant. Remember to configure the uppermost node of the flow with
  the IP of the NodeMCU.
  