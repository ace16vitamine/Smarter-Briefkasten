# esp8266-briefkasten
Smarter Briefkasten mit ESP8266 und MQTT WLAN. Das Modul wird in den Briefkasten eingebaut und versendet den inneren Abstand über MQTT an einen Broker. Von dort kann z.B. mittels OpenHAB der Wert verarbeitet werden um über neue Briefe im Briefksten zu benachrichtigen.

WICHTIG: Der HC-SR04 arbeitet mit 5V. Da der esp8266 nur 3V hat, wird ein Step Up Converter benötigt! Dieser wird zwischen esp8266 und HC-SR04 gelötet.

--------

Einkaufsliste Hardware:


ESP8266: https://amzn.to/3Z5wwCA

Ultraschallsensor: https://amzn.to/40jMiKL

Strom: https://amzn.to/3n0rdXI

StepUp Converter (3,7V auf 5V): https://amzn.to/3JODUO0


--------


Code befindet sich in main.ino


--------


Wire Connecting the HC-SR04 Module


HC-SR04 <-> ESP8266

VCC <-> VIN

Trig <-> D6

Echo <-> D5

GND <-> G



Wire wakeup the ESP from DeepSleep with internal RTC

ESP8266 <-> ESP8266

D0 <-> RST
                                      

