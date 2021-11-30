#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define moisturePin 4 //A0  //4  // used for ESP32
#define smokePin 5 //A1    //5  // used for ESP32


// Timings
unsigned long previousMillis1 = 0;     // will store last time LED was updated
unsigned long previousMillis2 = 0;     // will store last time LED was updated
const long smokeInterval = 1000;  
const long moistureInterval = 5000;  

float sensorValue;

void setup() { 
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= moistureInterval) {
    previousMillis2 = currentMillis;
    sensorValue = analogRead(moisturePin);
    Serial.print("moisture level: ");
    Serial.print(sensorValue);
  }
  if (currentMillis - previousMillis1 >= smokeInterval) {
    previousMillis1 = currentMillis;
    sensorValue = analogRead(smokePin);
    Serial.print(" Smoke level: ");
    Serial.println(sensorValue);
  }
  

} 
