
// Code für den smarten Briefkasten
// 15.03.2023 Stefan Eggert

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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
const int trigPin = 12;
const int echoPin = 14;

long duration;
float distanceCm;

WiFiClient espClient;
PubSubClient client(espClient); 

void setup() {
  
Serial.begin(9600);
delay(10);

// Sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

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
  // Serial.write(cr); 

  Serial.println("Messung starten...");

}

void loop() {

// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
// Calculate the distance
  distanceCm = duration * 0.03432 / 2; 
 
// Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
 
// MQTT
  char msgBuffer[20]; 
  connectToMQTT();
  client.publish("/esp/briefe/post", dtostrf(distanceCm, 6, 2, msgBuffer));
             
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

