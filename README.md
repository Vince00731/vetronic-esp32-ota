# VETRONIC ESP32 OTA

[![GitHub release](https://img.shields.io/github/v/release/Vince00731/vetronic-esp32-ota)](https://github.com/Vince00731/vetronic-esp32-ota/releases)
[![arduino-library-badge](https://www.ardu-badge.com/badge/AutoConnect.svg?)](https://www.ardu-badge.com/AutoConnect)
[![License](https://img.shields.io/github/license/Vince00731/vetronic-esp32-ota)](https://github.com/Vince00731/vetronic-esp32-ota/blob/master/LICENSE)

Permet le contrôle à distance de la borne de recharge VE-TRONIC WB-01(http://ve-tronic.fr/store/wallbox) par Jeedom(https://www.jeedom.com) via un ESP32.

## Overview

To the dynamic configuration for joining to WLAN with SSID and PSK accordingly. It an Arduino library united with *ESP8266WebServer* class of ESP8266 or *WebServer* class of ESP32.
Easily implementing the Web interface constituting the WLAN for ESP8266/ESP32 WiFi connection. With this library to make a sketch easily which connects from ESP8266/ESP32 to the access point at runtime by the web interface without hard-coded SSID and password.

<div align="center"><img alt="Overview" width="460" src="docs/images/ov.png" />&emsp;&emsp;&emsp;<img alt="Captiveportal" width="182" src="docs/images/ov.gif" /></div>

### No need pre-coded SSID &amp; password

It is no needed hard-coding in advance the SSID and Password into the sketch to connect between ESP8266/ESP32 and WLAN. You can input SSID &amp; Password from a smartphone via the web interface at runtime.

### Simple usage

AutoConnect control screen will be displayed automatically for establishing new connections. It aids by the <a href="https://en.wikipedia.org/wiki/Captive_portal">captive portal</a> when vested the connection cannot be detected.<br>By using the [AutoConnect menu](https://hieromon.github.io/AutoConnect/menu.html), to manage the connections convenient.

### Store the established connection

The connection authentication data as credentials are saved automatically in EEPROM of ESP8266/ESP32 and You can select the past SSID from the [AutoConnect menu](https://hieromon.github.io/AutoConnect/menu.html).

### Easy to embed in

AutoConnect can be embedded easily into your sketch, just "**begin**" and "**handleClient**".

### Lives with the your sketches

The sketches which provide the web page using ESP8266WebServer/WebServer there is, AutoConnect will not disturb it. AutoConnect can use an already instantiated ESP8266WebServer object(ESP8266) or WebServer object(ESP32), or itself can assign it.

### Easily add user-owned web screen and  parameters <sup><sub>ENHANCED w/ v0.9.7</sub></sup>

You can easily add your own web screen that can consist of representative HTML elements as the styled TEXT, INPUT, BUTTON, CHECKBOX, RADIO, SELECT, SUBMIT into the menu. It can be invoked from the AutoConnect menu and parameters can be passed.

### Just loading the JSON description <sup><sub>ENHANCED w/ v0.9.7</sub></sup>

These HTML elements that make up the user-owned screen can be easily loaded from the JSON description stored in PROGMEM, SPIFFS or SD.

<img width="40%" src="mkdocs/images/aux_json.png">&emsp;
<img width="32px" src="mkdocs/images/arrow_right.png" align="top">&emsp;
<img width="30%" height="400px" src="mkdocs/images/AutoConnectAux.gif">

### Quick and easy to equip the OTA update feature <sup><sub>ENHANCED w/ v1.0.0</sub></sup>

You can quickly and easily equip the [OTA update feature](https://hieromon.github.io/AutoConnect/otaupdate.html) to your sketch and also you can operate the firmware update process via OTA from AutoConnect menu.

## Supported hardware

Apply the [Arduino core](https://github.com/esp8266/Arduino) of the ESP8266 Community.

- Generic ESP8266 modules
- Adafruit HUZZAH ESP8266 (ESP-12)
- ESP-WROOM-02
- Heltec WiFi Kit 8
- NodeMCU 0.9 (ESP-12) / NodeMCU 1.0 (ESP-12E)
- Olimex MOD-WIFI-ESP8266
- SparkFun Thing
- SweetPea ESP-210

Alter the platform applying the [arduino-esp32](https://github.com/espressif/arduino-esp32) for the ESP32 modules.

- ESP32Dev Board
- SparkFun ESP32 Thing
- WEMOS LOLIN D32
- Ai-Thinker NodeMCU-32S
- Heltec WiFi Kit 32
- M5Stack
- Unexpected Maker TinyPICO
- And other ESP32 modules supported by the Additional Board Manager URLs of the Arduino-IDE.

## Simple usage

### The AutoConnect menu

<img src="mkdocs/images/menu_r.png" width="200">&emsp;<img src="mkdocs/images/statistics.png" width="200">&emsp;<img src="mkdocs/images/confignew.png"
 width="200">&emsp;<img src="mkdocs/images/openssid.png" width="200">

### How embed the AutoConnect to the sketches you have

Most simple approach to applying AutoConnect for the existing sketches, follow the below steps.

<img src="mkdocs/images/beforeafter.png">

## More usages and Documentation

Full documentation is available on https://Hieromon.github.io/AutoConnect, some quick links at the list:

- The [Installation](https://hieromon.github.io/AutoConnect/index.html#installation) is the installation procedure and requirements for the library.
- [Getting started](https://hieromon.github.io/AutoConnect/gettingstarted.html) with the most simple sketch for using AutoConnect.
- The [Basic usage](https://hieromon.github.io/AutoConnect/basicusage.html) guides to using the library correctly.
- Details are explained in the [Advanced usage](https://hieromon.github.io/AutoConnect/advancedusage.html).
- Details and usage of custom Web pages are explained in the [Custom Web pages](https://hieromon.github.io/AutoConnect/acintro.html).
- The [API reference](https://hieromon.github.io/AutoConnect/api.html) describes the AutoConnect functions specification.
- There are hints in [Examples](https://hieromon.github.io/AutoConnect/howtoembed.html) for making sketches with AutoConnect.
- [FAQ](https://hieromon.github.io/AutoConnect/faq.html).

## Change log

### [1.0] 5 août 2023
- Première version


## License

License under the [MIT license](LICENSE).
