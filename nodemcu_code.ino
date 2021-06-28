#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Ticker.h>

WiFiClientSecure espClient;   
PubSubClient client(espClient);
Ticker mqttReconnectTimer;

const char* ssid = "asd";
const char* password =  "amasonkez";
const char* mqttServer = "4be20a833dfa43c591120e0543b9f9e6.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char* mqttUser = "pengus";
const char* mqttPassword = "pattes1.K";
 
// Temperature MQTT Topics
#define MQTT_PUB_TEMP "esp/dht/temperature"
#define MQTT_PUB_HUM "esp/dht/humidity"

// Digital pin connected to the DHT sensor
#define DHTPIN 14  
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);
// Variables to hold sensor readings
float temp;
float hum;
unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 300000;        // Interval at which to publish sensor readings

/*
******** WiFi Connection ********
*/
Ticker wifiReconnectTimer;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password); 
}
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
 // connectToMqtt();
}
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}



void setup() {
  
  Serial.begin(115200);
  dht.begin();
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  connectToWifi();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  espClient.setInsecure();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  

 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}
 
void loop() {
  unsigned long currentMillis = millis(); 
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New DHT sensor readings
    hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();
    
    bool res = client.publish(MQTT_PUB_TEMP, String(temp).c_str());
    Serial.printf("Message: %.2f %s \n", temp, String(res).c_str());
    client.publish(MQTT_PUB_HUM, String(hum).c_str());
    Serial.printf("Message: %.2f \n", hum);
  }
  client.loop();
}