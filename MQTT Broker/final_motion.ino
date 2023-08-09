#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//#include <SoftwareSerial.h>
#include <math.h>
//#include <M5Core2.h>

boolean motion = true;

double roll, pitch, yaw;

const int x_out = A0; /* connect x_out of module to A1 of UNO board */
const int y_out = A3; /* connect y_out of module to A2 of UNO board */
const int z_out = A6; /* connect z_out of module to A3 of UNO board */

// Replace the next variables with your SSID/Password combination
const char* ssid = "SG4-3202";
const char* password = "SG4-3202";
const int relay = 26;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
int _moisture,sensor_analog;
const int sensor_pin = A0;

const char* mqtt_server = "mqtt.things.ph";
StaticJsonDocument<5000> payload;
StaticJsonDocument<5000> payload_fields;
char combo[5000];


WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
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
  Serial.println(messageTemp);

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "espCAM") {
    Serial.print("Changing output to ");
    if(messageTemp.substring(59, 60) == "1"){
//      digitalWrite(relay, HIGH);
      motion = true;
      Serial.println("high, off");
    }
    else if (messageTemp.substring(59, 60) == "0") {
//      digitalWrite(relay, LOW);
      motion = false;
      Serial.println("low, on");
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

void printEvent(sensors_event_t* event) {
    int x_adc_value, y_adc_value, z_adc_value;
    double x_g_value, y_g_value, z_g_value;
 
    x_adc_value = (analogRead(x_out)-2145)/5 * -1; /* Digital value of voltage on x_out pin */
    y_adc_value = (analogRead(y_out)-1790)/4 * -1; /* Digital value of voltage on y_out pin */
    z_adc_value = (analogRead(z_out)-2228)/4*-1; /* Digital value of voltage on z_out pin */
//    Serial.print("x = ");
//    Serial.print(x_adc_value);
//    Serial.print("\t\t");
//    Serial.print("y = ");
//    Serial.print(y_adc_value);
//    Serial.print("\t\t");
//    Serial.print("z = ");
//    Serial.print(z_adc_value);
//    Serial.println("");
   
    x_g_value = ( ( ( (double)(x_adc_value * 3.3)/4096) - 1.65 ) / 0.330 ); /* Acceleration in x-direction in g units */
    y_g_value = ( ( ( (double)(y_adc_value * 3.3)/4096) - 1.65 ) / 0.330 ); /* Acceleration in y-direction in g units */
    z_g_value = ( ( ( (double)(z_adc_value * 3.3)/4096) - 1.80 ) / 0.330 ); /* Acceleration in z-direction in g units */
 
    roll = ( ( (atan2(y_g_value,z_g_value) * 180) / 3.14 ) + 180 ); /* Formula for roll */
    pitch = ( ( (atan2(z_g_value,x_g_value) * 180) / 3.14 ) + 180 ); /* Formula for pitch */
    yaw = ( ( (atan2(x_g_value,y_g_value) * 180) / 3.14 ) + 180 ); /* Formula for yaw */

    payload_fields["tilt"] = x_g_value;

    Serial.print(motion);

    if (motion == true) {
      digitalWrite(relay, HIGH);
    } else {
      if(z_adc_value > 15 || z_adc_value < -15) {
        digitalWrite(relay, HIGH);
      } else {
        if (_moisture < 15) {
          digitalWrite(relay, LOW);
        } else {
          digitalWrite(relay, HIGH);
        }
      }
    }    

      sensor_analog = analogRead(sensor_pin);
      _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
      payload_fields["moisture"] = _moisture;
//      Serial.print("MOISTURE: "); Serial.println(_moisture);
      payload_fields["tilt"] = z_adc_value;
      // payload_fields["ytilt"] = y;
      payload["hardware_serial"] = "espRASPI";
      payload["payload_fields"] = payload_fields;
      serializeJson(payload, combo);
      client.publish("espRASPI", combo);
      delay(200);
   
   }

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  printEvent(&orientationData);
  //printEvent(&angVelocityData);
  //printEvent(&linearAccelData);
  printEvent(&magnetometerData);
  //printEvent(&accelerometerData);
  //printEvent(&gravityData);

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
//  Serial.println();
//  Serial.print(" Gyro=");
//  Serial.print(gyro);
  //Serial.print(" Accel=");
  //Serial.print(accel);
  // Serial.print(" Mag=");
  //Serial.println(mag);
 
//  Serial.println("--");
  
  Serial.setTimeout(50);
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    //Serial.println(combo);
  }
}
