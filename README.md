# CorO₂Sens - MH-z19B variant

This is a fork of https://github.com/kmetz/coro2sens to enable usage of the other mentioned
CO₂ sensor: MH-z19b.  
I used this board (relevant for the pins, always check your pins!): https://joy-it.net/de/products/SBC-NodeMCU-ESP32
---

Build a simple device that warns if CO₂ concentration in a room becomes a risk for COVID-19 aerosol infections.

- Measures CO₂ concentration in room air.
- Controls an RGB LED (green, yellow, red, like a traffic light).
- A buzzer can be connected that alarms if levels are critical.
- Also opens a WiFi portal which shows current readings and a graph (not connected to the internet).
- Can be built for ~ $60 / 50€ (parts cost).

This project was heavily inspired by [ideas from Umwelt-Campus Birkenfeld](https://www.umwelt-campus.de/forschung/projekte/iot-werkstatt/ideen-zur-corona-krise).

You can also find a good overview of the topic by Rainer Winkler here: [Recommendations for use of CO2 sensors to control room air quality during the COVID-19 pandemic](https://medium.com/@rainer.winkler.poaceae/recommendations-for-use-of-co2-sensors-to-control-room-air-quality-during-the-covid-19-pandemic-c04cac6644d0).

![coro2sens overview](coro2sens.jpeg)


## Sensors
- The sensor used here is the Sensirion SCD30 (around $50 / 40€) which is optionally augmented by a BME280 pressure sensor to improve accuracy.
- [Look here](https://github.com/RainerWinkler/CO2-Measurement-simple) if you want to use MH-Z19B sensors.


## Threshold values
| LED color                 |CO₂ concentration |
|:--------------------------|:----------------------------|
| Green ("all good")        | < 800 ppm                  |
| Yellow ("open windows")   | 800 – 1000 ppm             |
| Red ("leave room")        | \> 1000 ppm                 |

Based on a [Recommendation from the REHVA](https://www.rehva.eu/fileadmin/user_upload/REHVA_COVID-19_guidance_document_V3_03082020.pdf)
(Federation of European Heating, Ventilation and Air Conditioning associations, [rehva.eu](https://www.rehva.eu/))
for preventing COVID-19 aerosol spread, especially in schools.


## Web server
You can read current levels and a simple graph for the last hour by connecting to the WiFi `coro2sens` that is created.
Most devices will open a captive portal, immediately showing the data. You can also open `http://10.0.0.1/` in a browser.


## You need
1. Any ESP32 or ESP8266 board (like a [WEMOS D32](https://docs.wemos.cc/en/latest/d32/d32.html) (about $18 / 15€) or [WEMOS LOLIN D1 Mini](https://docs.wemos.cc/en/latest/d1/d1_mini.html) (about $7 / 6€)).  
ESP32 has bluetooth, for future expansion.
1. [Sensirion SCD30](https://www.sensirion.com/en/environmental-sensors/carbon-dioxide-sensors/carbon-dioxide-sensors-co2/) I<sup>2</sup>C carbon dioxide sensor module ([mouser](https://mouser.com/ProductDetail/Sensirion/SCD30?qs=rrS6PyfT74fdywu4FxpYjQ==), [digikey](https://www.digikey.com/product-detail/en/sensirion-ag/SCD30/1649-1098-ND/8445334)) (around $50 / 40€).
1. 1 [NeoPixel](https://www.adafruit.com/category/168) compatible RGB LED (WS2812B, like the V2 Flora RGB Smart NeoPixel LED, you can also remove one from a larger strip which might be cheaper).
1. A 3V piezo buzzer or a small speaker.
1. Optional (i didn't include one yet): [Bosch BME280](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/) I<sup>2</sup>C sensor module (like the GY-BME280 board), for  air pressure compensation, improves accuracy (less than $5 / 4€).   
1. ~~A nice case :) Make sure the sensor has enough air flow.~~


### Wiring

| ESP32 pin | goes to                          |
|:----------|:---------------------------------|
| 5V        | MH-z19b VIN, LED +               |
| GND       | MH-z19b GND, LED GND, Buzzer (-) |
| D26       | MH-z19b Rx                       |
| D32       | MH-z19b Tx                       |
| D23       | LED DIN                          |
| D19       | Buzzer (+, or "H")               |


### Flashing the ESP using [PlatfomIO](https://platformio.org/)
- Simply open the project, select your env (`esp12e` for ESP8266 / `nodemcu-32s` for ESP32) and run / upload.
- Or via command line:
  - `pio run -t -e nodemcu-32s upload` for ESP32.
- Libraries will be installed automatically.
- There is a small test suite to help with the wiring. Run it like this:
  - `pio test`

### Flash using the Arduino IDE
I didn't try that myseld, please see the original repo for further information
