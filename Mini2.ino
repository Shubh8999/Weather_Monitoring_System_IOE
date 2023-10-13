#include <dht.h>  // Include D
#define outPin 8            // Defines the pin number to which the DHT sensor is connected
#define airQualitySensorPin A4 // Defines the analog pin number to which the air quality sensor is connected
#define buzzerPin 9         // Defines the pin number to which the buzzer is connected

dht DHT;                    // Create a DHT obje3ct

int sensorValue;
int digitalValue;
int airQualityValue;
String str;


// Sensor pins
#define sensorPower 7
#define rainSensorPin 6

void setup() {
  pinMode(sensorPower, OUTPUT);

  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);

  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
}

void loop() {
  // Read data from DHT sensor
  int readData = DHT.read11(outPin);
  float t = DHT.temperature; // Read temperature
  float h = DHT.humidity;    // Read humidity

  // Print temperature and humidity readings
  Serial.print("DHT Sensor - Temperature = ");
  Serial.print(t);
  Serial.print("°C | ");
  Serial.print((t * 9.0) / 5.0 + 32.0); // Convert Celsius to Fahrenheit
  Serial.println("°F");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println("%");

  // Check if humidity is above 90% and activate the buzzer
  if (h == 95.0) {
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }

  // Read analog and digital sensor values
  sensorValue = analogRead(0); // Read analog input pin 0
  digitalValue = digitalRead(2); // Read digital input pin 2


  // Control an LED on pin 13 based on the analog sensor value
  if (sensorValue > 200) {
    digitalWrite(10, HIGH); // Turn on the LED on pin 13
  } else {
    digitalWrite(10, LOW); // Turn off the LED on pin 13
  }

  // Read air quality sensor data
  int airQualityValue = analogRead(airQualitySensorPin);
  Serial.print("Air Quality: ");
  Serial.print(airQualityValue, DEC);
  Serial.println(" PPM");

  // Read the rain sensor data and determine status
  int rainSensorValue = readRainSensor();

  
  int rainstatus = 0;
  if (rainSensorValue == LOW) {
    Serial.println("Status: It's raining");
    rainstatus = 1;
  } else {
    Serial.println("Status: Clear");
    rainstatus = 0;

  }

  Serial.println("Data sent to ESP");
  str = String("Temperature:" + String(t, 2) + " " + "Humidity:" + String(h, 2) + " " + "Air_Quality:" + String(airQualityValue) + " " + "Rain_status:" + String(rainstatus));

  Serial.println(str);


  delay(4000); // Wait for 2 seconds before taking the next reading
}

// Function to read the rain sensor
int readRainSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);  // Allow power to settle
  int val = digitalRead(rainSensorPin);  // Read the sensor output
  digitalWrite(sensorPower, LOW);  // Turn the sensor OFF
  return val;  // Return the value
}