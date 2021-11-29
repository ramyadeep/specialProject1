//#define SensorPin A0  // used for Arduino and ESP8266

#define moisturePin 4  // used for ESP32
#define smokePin 5     // used for ESP32


// Timings
unsigned long previousMillis1 = 0;     // will store last time LED was updated
unsigned long previousMillis2 = 0;     // will store last time LED was updated
const long smokeInterval = 1000;  
const long moistureInterval = 5000;  

float sensorValue;

void setup() { 
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= smokeInterval) {
    previousMillis1 = currentMillis;
    sensorValue = analogRead(smokePin);
    Serial.print("Smoke level: ");
    Serial.print(sensorValue);
  }
  if (currentMillis - previousMillis2 >= moistureInterval) {
    previousMillis2 = currentMillis;
    sensorValue = analogRead(moisturePin);
    Serial.print("moisture level: ");
    Serial.println(sensorValue);
  }
} 
