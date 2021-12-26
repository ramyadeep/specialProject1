// Include libraries
#include <SimpleDHT.h>
#include <SPI.h>
#include <SD.h>


// Define connections to IO
#define moisturePin A1  //4 used for ESP32
#define smokePin A2     //5 used for ESP32
#define pinDHT11 2
#define sdPin 4


// Event Timings
const int smokeInterval = 1000;  
const int moistureInterval = 5000;
const int logInterval = 10000;
   
unsigned long previousMillis1 = 0;     
unsigned long previousMillis2 = 0;  
unsigned long previousMillis3 = 0;    

// sensor values
float smoke,moisture;
byte temperature = 0;
byte airHumidity = 0;

// Initializing objects
SimpleDHT11 dht11(pinDHT11);


//Functions
void logData(){
  // see if the card is present and can be initialized:
  if (!SD.begin(sdPin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  File dataFile = SD.open("data.csv", FILE_WRITE);
   // if the file is available, write to it:
  if (dataFile) {
    String sensor_data =(String) moisture+","+ (String) temperature+"," + (String)airHumidity+"\n";
    dataFile.println(sensor_data);
    dataFile.close();
    Serial.println("Success");
  }
  else { 
    // if the file isn't open, pop up an error:
    Serial.println("error opening data.csv");
  }
}

void setup() { 
  Serial.begin(9600);
//  if (!SD.begin(sdPin)) {
//    Serial.println("Card failed, or not present");
//    // don't do anything more:
//    while (1);
//  }
//  Serial.println("card initialized.");
  
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= moistureInterval) {
    previousMillis1 = currentMillis;
    moisture = map(analogRead(moisturePin),0,1024,0,100);
    Serial.print("moisture level: ");
    Serial.print(moisture);
    Serial.print(" % ");
    

    int err = SimpleDHTErrSuccess;
    if((err = dht11.read(&temperature, &airHumidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    }else{
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C");
    }
  }
  if (currentMillis - previousMillis2 >= smokeInterval) {
    previousMillis2 = currentMillis;
    smoke = 100 - map(analogRead(smokePin),0,1024,0,100);
    Serial.print(" Smoke level: ");
    Serial.print(smoke);
    Serial.println(" %");
  }

  if (currentMillis - previousMillis3 >= logInterval) {
    previousMillis3 = currentMillis;
    // write logging codes here
  }
} 
