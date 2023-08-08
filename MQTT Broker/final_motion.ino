#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "SG4-3202";
const char* password = "SG4-3202";
const int relay = 26
;

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "mqtt.things.ph";
StaticJsonDocument<5000> payload;
StaticJsonDocument<5000> payload_fields;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char combo[200];
//uncomment the following lines if you're using SPI
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)  
//  if (!bme.begin(0x76)) {
//    Serial.println("Could not find a valid BME280 sensor, check wiring!");
//    while (1);
//  }
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(relay, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
//  Serial.print("Message arrived on topic: ");
//  Serial.print(topic);
//  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
//    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp.substring(59, 60));

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "espCAM") {
    Serial.print("Changing output to ");
    if(messageTemp.substring(59, 60) == "1"){
      digitalWrite(relay, HIGH);
      Serial.println("off");
    }
    else if (messageTemp.substring(59, 60) == "0") {
      digitalWrite(relay, LOW);
      Serial.println("on");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client","64c93f454117d46beb4677d9","UAnvMtslycoTolEQ2r6HahUq")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("espCAM");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
//    payload_fields["led"] = 1;
//    payload_fields["switch"] = 1;
//    payload_fields["payload_fields"] = 1;
//
//    payload["hardware_serial"] = "ESP_32";
//    payload["payload_fields"] = payload_fields;

   // serializeJson(payload, Serial);
//    serializeJson(payload, combo);
//    client.publish("ESP_32", combo);
    Serial.println(combo);
  }
}
