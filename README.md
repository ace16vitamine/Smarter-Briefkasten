# Smarter Briefkasen.

Vorweg: Version 1 und Version 2 arbeiten mit WLAN und MQTT. Der Unterschied: Version 1 nutzt einen Ultraschallsensor, Version 2 zwei Reedkontakte. Ich finde die Reedkontakte besser, da nicht immer einzelne Briefe mit dem Ultraschallsensor erfasst werden. Zudem ist die Lösung günstiger und es wird weniger Hardware benötigt.

--------
Beschreibung Version 1: Smarter Briefkasten mit ESP8266 und MQTT WLAN. Das Modul wird in den Briefkasten eingebaut und versendet den inneren Abstand über MQTT an einen Broker. Von dort kann z.B. mittels OpenHAB der Wert verarbeitet werden um über neue Briefe im Briefksten zu benachrichtigen.

WICHTIG: Der HC-SR04 arbeitet mit 5V. Da der esp8266 nur 3V hat, wird ein Step Up Converter benötigt! Dieser wird zwischen esp8266 und HC-SR04 gelötet.

Einkaufsliste: 

ESP8266: https://amzn.to/3Z5wwCA

Ultraschallsensor: https://amzn.to/40jMiKL

Strom: https://amzn.to/3n0rdXI

StepUp Converter (3,7V auf 5V): https://amzn.to/3JODUO0



--------
Beschreibung Version 2: Smarter Briefkasten mit ESP32 und MQTT WLAN. Das Modul wird in den Briefkasten eingebaut und versendet wie folgt die Öffnungen über MQTT an einen Broker. Von dort kann z.B. mittels OpenHAB der Wert verarbeitet werden um über neue Briefe im Briefksten zu benachrichtigen.

Logik: 

1) Schalter 1: Post wird eingeworden
2) Deaktivieren Schalter 1
3) Aktivieren Schalter 2
4) DeepSleep
5) Schalter 2: Post wird entnommen
6) Deaktivieren Schalter 2
7) Aktivieren Schalter 1
8) DeepSleep

Einkaufsliste: 

ESP32: https://amzn.to/42AxFoy

Strom: https://amzn.to/3n0rdXI

Reed Door Switch: https://amzn.to/3z5sMGu

--------


Code befindet sich in main.ino


--------


Wire Connecting the HC-SR04 Module für Version 1


HC-SR04 <-> ESP8266

VCC <-> VIN

Trig <-> D6

Echo <-> D5

GND <-> G



Wire wakeup the ESP from DeepSleep with internal RTC

ESP8266 <-> ESP8266

D0 <-> RST
                                      

