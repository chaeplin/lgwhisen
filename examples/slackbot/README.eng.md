Control LG WHISEN Air Conditioners with slackbot
===================================

![conn](./pics/conn.jpg)
![slackbot](./pics/slackbot.jpg)

## Can do
- On, Off
- Timer on, Timer off
- set temperature
- set wind flow
- report status
- adjust/report temp/flow/on/off when changed by ir remote controller 

## 명령
- help
```
on -> ac on
off -> ac off
ton -> timer start with ac on
[18 ~ 30] -> temperature set
[0 ~ 3] -> flow set(low/mid/high/change)
check -> report ac status
```

## Limitation
- lgWhisen.h only works with LG WHISEN Air Conditioners
- heating is not tested

## Thanks to: 
- Uri Shaked @UriShaked : Arduino Real-Time Slack Bot  [https://github.com/urish/arduino-slack-bot](https://github.com/urish/arduino-slack-bot) [https://medium.com/@urish/how-to-connect-your-t-shirt-to-slack-using-arduino-90761201d70f#.owc9mpvvk](https://medium.com/@urish/how-to-connect-your-t-shirt-to-slack-using-arduino-90761201d70f#.owc9mpvvk)

- Arduino-IRremote : https://github.com/z3t0/Arduino-IRremote
- Mark Szabo : https://github.com/markszabo/IRremoteESP8266


## Sketch
[https://github.com/chaeplin/lgwhisen/blob/master/examples/slackbot/slackbot.ino](https://github.com/chaeplin/lgwhisen/blob/master/examples/slackbot/slackbot.ino)

### Device
- esp8266(nodemcu is used)
- IR RX : TSOP38238 // https://learn.adafruit.com/ir-sensor
- IR TX : IR LED(38KHz. 940nm)

nodemcu | ir rx | ir tx
--------|-------|--------
![nodemcu](./pics/nodemcuv1.jpg) | ![irrx](./pics/TSOP38238-ir-rx.jpg) | ![irtx](./pics/ir-tx.jpg)

### Libs
- WebSocketsClient : [https://github.com/Links2004/arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets)
- ArduinoJson : [https://github.com/bblanchon/ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- lgWhisen : [https://github.com/chaeplin/lgwhisen](https://github.com/chaeplin/lgwhisen)
- IRremoteESP8266 : [https://github.com/markszabo/IRremoteESP8266](https://github.com/markszabo/IRremoteESP8266)

### Connection
- RX : TSOP38238's out to D5/GPIO14
- TX : IR LED's + or SIG to D4/GPIO2

### Config for WIFI and slack
- slackbot api token
```
#define WIFI_SSID       "wifi-name"
#define WIFI_PASSWORD   "wifi-password"
#define OTA_PASSWORD    "ota-password"

// https://api.slack.com/bot-users
#define SLACK_BOT_TOKEN "put-your-slack-token-here"
```

- after initial connection test to slack, send a message to bot, channel, user, and team can be found on serial monitor.

```
// sned a msg to bot using slack app or web.
// check Arduino serial monitor, channel, user and team cam be found 
#define SLACK_CHANNEL "xxxxxx"
#define SLACK_USER "xxxxxxx"
#define SLACK_TEAM "xxxxxx"

```

- Config for Air conditoner
```
#define IR_RX_PIN 14 
#define IR_TX_PIN 2
#define AC_CONF_TYPE 1      // 0 : Tower, 1: Wall
#define AC_CONF_HEATING 0   // 0 : Cooling, 1 : Heating
#define AC_CONF_ON_MIN 30   // timed on / min
#define AC_CONF_OFF_MIN 20  // timed off / min
// ex) 30 min on, 20 min off, 30 min on, 20 min off....
```

- initial temperature and wind flow
```
  ir_data.ac_temp     = 27; // temperature
  ir_data.ac_flow     = 1;  // wind, 0 : low, 1 : mid, 2 : high, if ac type is wall -> 3: change
```


* ir decoding : [https://github.com/chaeplin/lgwhisen/blob/master/decoding.md](https://github.com/chaeplin/lgwhisen/blob/master/decoding.md)
