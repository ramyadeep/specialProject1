//#include "soc/soc.h"
//#include "soc/rtc_cntl_reg.h"

#define moisturePin A1  //4  // used for ESP32
#define smokePin A2    //5  // used for ESP32


// Timings
unsigned long previousMillis1 = 0;     // will store last time LED was updated
unsigned long previousMillis2 = 0;     // will store last time LED was updated
const long smokeInterval = 1000;  
const long moistureInterval = 5000;  

float sensorValue;

void setup() { 
//  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= moistureInterval) {
    previousMillis2 = currentMillis;
    sensorValue = map(analogRead(moisturePin),0,1024,0,100);
    Serial.print("moisture level: ");
    Serial.print(sensorValue);
    Serial.print(" %");
  }
  if (currentMillis - previousMillis1 >= smokeInterval) {
    previousMillis1 = currentMillis;
    sensorValue = map(analogRead(smokePin),0,1024,0,100);
    Serial.print(" Smoke level: ");
    Serial.print(sensorValue);
    Serial.println(" %");
  }
  

} 
