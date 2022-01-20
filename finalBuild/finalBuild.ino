// Include libraries
#include <SimpleDHT.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Define Security credentials
#define WIFISSID "BUGS&GLITCHES"                     // Wifi SSID
#define PASSWORD "abcd1234"                          // wifi password 
#define TOKEN "BBFF-ydNXCnR9JeaCChHshl1n222YEz4UML"  // Ubidots access TOKEN
#define MQTT_CLIENT_NAME "RamyadeepMQTTESP3"         // MQTT client Name,visible to the broker

// Define connections to IO
#define moisturePin 4  //4 used for ESP32
#define smokePin 5     //5 used for ESP32
#define pinDHT11 2
#define sdPin 4

// broker domain and port
const char mqttBroker[]  = "things.ubidots.com";
const int port = 1883;
char str_sensor[10];                                 // Space to store values to send

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


// Initializing library objects
WiFiClient ubidots;
PubSubClient client(ubidots);
SimpleDHT11 dht11(pinDHT11);

// Create local sensor log files
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



// callback function to handle requests
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  String state = message.substring(10,11);
  if (state == "1") {
    Serial.println("Turn on Relay");
  } else {
    Serial.println("Turn off Relay");
  }
//  Serial.write(payload, length);
  Serial.println("Callback triggred");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME,TOKEN, "")) {
      Serial.println("Connected");
      client.subscribe("/v1.6/devices/esp32/iirragate-field");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}




void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println();
  Serial.print("Wait for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

  while(!SD.begin(sdPin)){
  if (!SD.begin(sdPin)) {
    Serial.println("Card failed, or not present");
    Serial.println("Try Again in 2 seconds");
    // don't do anything more:
  }
  Serial.println("card initialized.");
  }
}

void loop() {
  if (!client.connected()) {
    client.subscribe("/v1.6/devices/esp32/irragate-field");   
    reconnect();
  }
  
  Serial.println("Publishing data to Ubidots Cloud\n");
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis1 >= moistureInterval) {
    previousMillis1 = currentMillis;
    
    moisture = map(analogRead(moisturePin),0,1024,0,100);
    // publish moisture 
    dtostrf(moisture, 4, 2, str_sensor);
    client.publish("/v1.6/devices/esp32/soil-moisture",str_sensor);
    Serial.print("TOPIC: /v1.6/devices/esp32/soil-moisture ");

    // publish ph level data
    // simulating real world values
    dtostrf(random(4,5), 4, 2, str_sensor);
    client.publish("/v1.6/devices/esp32/soil-ph-level",str_sensor);
    Serial.print("TOPIC: /v1.6/devices/esp32/soil-ph-level ");

   
    dtostrf(random(70,75), 4, 2, str_sensor);
    client.publish("/v1.6/devices/esp32/sun-light",str_sensor);
    Serial.print("TOPIC: /v1.6/devices/esp32/sun-light ");
    
    // reading temperature data form dht11 sensor
    int err = SimpleDHTErrSuccess;
    if((err = dht11.read(&temperature, &airHumidity, NULL)) == SimpleDHTErrSuccess) {
      //publish temperature
      dtostrf(temperature, 4, 2, str_sensor);
      client.publish("/v1.6/devices/esp32/temperature",str_sensor);
      Serial.print("TOPIC: /v1.6/devices/esp32/temperature ");
    }
  }
  if (currentMillis - previousMillis2 >= smokeInterval) {
    previousMillis2 = currentMillis;
    smoke = 100 - map(analogRead(smokePin),0,1024,0,100);
   
    Serial.print(smoke);
    // publish smoke 
    dtostrf(smoke, 4, 2, str_sensor);
    client.publish("/v1.6/devices/esp32/smoke-sensor",str_sensor);
    Serial.print("TOPIC: /v1.6/devices/esp32/smoke-sensor ");
  }

  if (currentMillis - previousMillis3 >= logInterval) {
    previousMillis3 = currentMillis;
    // logging codes here
    logData();
  }
  client.loop();
}
