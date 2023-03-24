// Smarter Briefkasten Version 2
// Verbesserte Versio


#define DOOR_SENSOR_PIN_REIN  27  // ESP32 pin GIOP19 connected to door sensor's pin
#define DOOR_SENSOR_PIN_RAUS  25  // ESP32 pin GIOP19 connected to door sensor's pin
#include <PubSubClient.h>
#include <WiFi.h>

// int TIME_TO_SLEEP = 600;
int doorState_REIN;
int doorState_RAUS;

// WLAN
const char* ssid = "SSID"; //SSID
const char* password = "Kennwort"; //Kennwort

WiFiClient espClient;
PubSubClient client(espClient); 


// MQTT 
const char* clientId = "ESP-Briefkasten";
const char* mqtt_server = "192.168.1.1"; // MQTT Server 
const int mqtt_port = 1883; // MQTT Port
const char* mqtt_user = "user"; //MQTT User
const char* mqtt_password = "passwort"; //MQTT Passwort
const char* mqttstring = "";


void setup() {

  // Türsensoren
  Serial.begin(9600);                          // initialize serial
  pinMode(DOOR_SENSOR_PIN_REIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  pinMode(DOOR_SENSOR_PIN_RAUS, INPUT_PULLUP); // set ESP32 pin to input pull-up mode

 esp_sleep_enable_ext0_wakeup(GPIO_NUM_25,1 );  // Set initial -> Wird benötigt um in die Schleife zu kommen
  
}


void loop() {
  doorState_REIN = digitalRead(DOOR_SENSOR_PIN_REIN); // read state
  doorState_RAUS = digitalRead(DOOR_SENSOR_PIN_RAUS); // read state


  if (doorState_REIN == HIGH) {
    Serial.println("REIN is open");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_25,1 );
    
    WiFiC();
    connectToMQTT();
    client.publish("/esp/briefkasten/p_neu","1");
  } else {
    Serial.println("REIN is closed");
  }

  if (doorState_RAUS == HIGH) {
    Serial.println("RAUS is open");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,1 );
    WiFiC();
    connectToMQTT();
    client.publish("/esp/briefkasten/p_neu","0");

  } else {
    Serial.println("RAUS is closed");
  }


delay(1000); // Wird benötigt um alles zum Abschluss zu bekommen
startDeepSleep();

}




void startDeepSleep(){
  Serial.println("Going to sleep... NOW");
  Serial.flush();
  esp_deep_sleep_start();
}


void connectToMQTT() {

 client.setServer(mqtt_server, mqtt_port);
  
  if (client.connect(clientId , mqtt_user, mqtt_password)) {
    Serial.println("connected");
  }
}

void WiFiC() {

  Serial.print("Verbinden mit: "); 
  Serial.println(ssid);
  delay(1000);
  WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
Serial.print("WL not connected, Connect now");
WiFi.begin(ssid, password);
delay(10000); // 10k damit die Verbindung vollständig aufgebaut ist bevor ein Reconnect kommt 

}
  Serial.println("");
  Serial.println("WiFi verbunden");

  // Print the IP address
  Serial.print(WiFi.localIP()) ;
  Serial.println(""); 

}




