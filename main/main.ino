
// Code für den smarten Briefkasten
// 15.03.2023 Stefan Eggert
// Add: NewPing Lib, https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!connection-example

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NewPing.h>

// MQTT 
const char* clientId = "ESP-Briefkasten";
const char* mqtt_server = "192.168.1.1"; // MQTT Server 
const int mqtt_port = 1883; // MQTT Port
const char* mqtt_user = "user"; //MQTT User
const char* mqtt_password = "passwort"; //MQTT Passwort
const char* mqttstring = "";
int cr = 13;

// WLAN
const char* ssid = "SSID"; //SSID
const char* password = "Kennwort"; //Kennwort

// Sensor

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     14  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

WiFiClient espClient;
PubSubClient client(espClient); 

void setup() {
  
Serial.begin(9600);
delay(10);

// Mit Wifi verbinden
  Serial.print("Verbinden mit: "); 
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi verbunden");

  // Print the IP address
  Serial.print(WiFi.localIP()) ;
  Serial.println(""); 

  Serial.println("Messung starten...");

}

void loop() {


// Sensor
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

// MQTT
  char msgBuffer[20]; 
  connectToMQTT();
  client.publish("/esp/briefe/post", dtostrf(sonar.ping_cm(), 6, 2, msgBuffer));

  delay(1000); //Verzögerung, damit MQTT Datenübertragung abgeschlossen wird
  Serial.println("Going to deep sleep...");
  ESP.deepSleep(60 * 1000000 ); /* Sleep für 60 Sekunden */

}

void connectToMQTT() {

 client.setServer(mqtt_server, mqtt_port);
  
  if (client.connect(clientId , mqtt_user, mqtt_password)) {
    Serial.println("connected");
  }
}

