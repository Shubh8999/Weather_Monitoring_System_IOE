#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "Secrets.h"

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

#define RXp2 16
#define TXp2 17

StaticJsonDocument<200> doc;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage() {
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // Serialize JSON data

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("incoming: ");
  Serial.println(topic);

  Serial.print("Received JSON data: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<200> receivedDoc;
  DeserializationError error = deserializeJson(receivedDoc, payload, length);

  if (error) {
    Serial.print("JSON parsing error: ");
    Serial.println(error.c_str());
  } else {
    const char* message = receivedDoc["message"];
    Serial.println("Message: " + String(message));
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  connectAWS();
}

void loop() {
  // Read data from Serial2 (Arduino) and forward it to AWS IoT
  if (Serial2.available()) {
    Serial.println("Message Received: ");
    String data = Serial2.readStringUntil('\n'); // Read a line of data from serial

    // Serial.println("Received data: " + data);

    float temperature;
    float humidity;
    int airQualityValue;
    int rainSensorValue;

    // Parse the data using sscanf
    int parsedValues = sscanf(data.c_str(), "Temperature:%f Humidity:%f Air_Quality:%d Rain_status:%d", &temperature, &humidity, &airQualityValue, &rainSensorValue);
    Serial.println(temperature);
    Serial.println(humidity);
    Serial.println(airQualityValue);
    Serial.println(rainSensorValue);

    // Check if sscanf successfully parsed all values
    if (parsedValues == 4) {
      // Add the parsed data to the JSON document
      doc["temperature"] = temperature;
      doc["humidity"] = humidity;
      doc["air_quality"] = airQualityValue;
      doc["rain_status"] = rainSensorValue;

      // Publish the sensor values to AWS IoT
      publishMessage();
    }
  }

  client.loop();
  delay(1000);
}
