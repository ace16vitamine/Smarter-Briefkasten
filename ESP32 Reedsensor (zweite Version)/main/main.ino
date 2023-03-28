// Smarter Briefkasten Version 2
// Verbesserte Version mit MQTT & OTA Updates (otadrive)

#define DOOR_SENSOR_PIN_REIN  27  // ESP32 pin GIOP19 connected to door sensor's pin
#define DOOR_SENSOR_PIN_RAUS  25  // ESP32 pin GIOP19 connected to door sensor's pin
#include <PubSubClient.h>
#include <WiFi.h>
#include <Credentials.h>                    // <---- REMOVE THIS LINE
#include <otadrive_esp.h>                   // <---- REMOVE ALL LINES WITH REMARK OTA IF YOU DO NOT WANT TO YOUSE OTADRIVE

int doorState_REIN;
int doorState_RAUS;
const char* clientId = "";
const char* mqttstringshort = "/esp/post/"; // Toppic -> Final Results are /TOPPIC/MAC
const char* this_version = "1.1.9";

// WLAN
WiFiClient espClient;
PubSubClient client(espClient); 

/* ENABLE THIS BLOCK TO COMPLETE YOUR WLAN AND MQTT SETTINGS
const char* ssid = "SSID"; //SSID
const char* wifi_password = "Kennwort"; //Kennwort

// MQTT 
const char* clientId = "ESP-Briefkasten";
const char* mqtt_server = "192.168.1.1"; // MQTT Server 
const int mqtt_port = 1883; // MQTT Port
const char* mqtt_user = "user"; //MQTT User
const char* mqtt_password = "passwort"; //MQTT Passwort
*/


void setup() {

  // Türsensoren
  Serial.begin(9600);                          // initialize serial
  pinMode(DOOR_SENSOR_PIN_REIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  pinMode(DOOR_SENSOR_PIN_RAUS, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  
  OTADRIVE.setInfo(ota_api_bk, "v@1.1.9");     // <---- REMOVE ALL LINES WITH REMARK OTA IF YOU DO NOT WANT TO YOUSE OTADRIVE

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,1 );  // Set initial -> Wird benötigt um in die Schleife zu kommen, beginnend mit Post wird eingeworfen (für den Reboot nach Update wichtig).
  
}


void loop() {
  doorState_REIN = digitalRead(DOOR_SENSOR_PIN_REIN); // read state
  doorState_RAUS = digitalRead(DOOR_SENSOR_PIN_RAUS); // read state

// Get Mac
  String wifiMacString = WiFi.macAddress();
  wifiMacString.replace(":", "");
  Serial.println(wifiMacString);

// String toppic = mqttstringshort + Mac

  String toppic = mqttstringshort + wifiMacString + "/result";
  String toppic_ip = mqttstringshort + wifiMacString + "/ip";
  String toppic_version = mqttstringshort + wifiMacString + "/version";



//Umwandeln in Char
 
  const char* mqtt_toppic=toppic.c_str();
  const char* mqtt_toppic_ip=toppic_ip.c_str();
  const char* mqtt_toppic_version=toppic_version.c_str();






  if (doorState_REIN == HIGH) {
    Serial.println("REIN is open");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_25,1 );
    
    WiFiC();
    connectToMQTT();

  client.publish(mqtt_toppic,"1");
  client.publish(mqtt_toppic_ip, (WiFi.localIP().toString().c_str()));
  client.publish(mqtt_toppic_version, this_version);


ota();   // <---- REMOVE ALL LINES WITH REMARK OTA IF YOU DO NOT WANT TO YOUSE OTADRIVE

   // client.publish("/esp/briefkasten/p_neu","1");
  } else {
    Serial.println("REIN is closed");
  }

  if (doorState_RAUS == HIGH) {
    Serial.println("RAUS is open");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,1 );
    WiFiC();
    connectToMQTT();

  client.publish(mqtt_toppic,"0");
  client.publish(mqtt_toppic_ip, (WiFi.localIP().toString().c_str()));
  client.publish(mqtt_toppic_version, this_version);

   ota();   // <---- REMOVE ALL LINES WITH REMARK OTA IF YOU DO NOT WANT TO YOUSE OTADRIVE

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
  WiFi.begin(ssid, wifi_password);

while (WiFi.status() != WL_CONNECTED) {
Serial.print("WL not connected, Connect now");
WiFi.begin(ssid, wifi_password);
delay(10000); // 10k damit die Verbindung vollständig aufgebaut ist bevor ein Reconnect kommt 

}
  Serial.println("");
  Serial.println("WiFi verbunden");

  // Print the IP address
  Serial.print(WiFi.localIP()) ;
  Serial.println(""); 

}





// START <---- REMOVE THIS BLOCK IF YOU DO NOT WANT TO YOUSE OTADRIVE
void ota()

{
  if(OTADRIVE.timeTick(30))
  {

auto uinf = OTADRIVE.updateFirmwareInfo();
if (uinf.available)
{
    
      OTADRIVE.updateFirmware();
      Serial.println("Rebooting")
      ESP.restart();
}


  
  }
}
// END <---- REMOVE ALL LINES WITH REMARK OTA IF YOU DO NOT WANT TO YOUSE OTADRIVE



