#include <WiFi.h>
#include <PubSubClient.h>

#define WIFISSID "BUGS&GLITCHES"                     // Put your WifiSSID here
#define PASSWORD "abcd1234"                          // Put your wifi password here
#define TOKEN "BBFF-ydNXCnR9JeaCChHshl1n222YEz4UML"  // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "RamyadeepMQTTESP3"         // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
                                                     //it should be a random and unique ascii string and different from all other devices
/****************************************
 * Define Constants
 ****************************************/

//#define VARIABLE_LABEL_SUBSCRIBE "relay"           // Assing the variable label


//#define relay 26                                   // Set the GPIO26 as RELAY

char mqttBroker[]  = "things.ubidots.com";
char topicSubscribe[100];
char str_sensor[10];                                 // Space to store values to send

/****************************************
 * Auxiliar Functions
 ****************************************/

WiFiClient ubidots;
PubSubClient client(ubidots);

//void callback(char* topic, byte* payload, unsigned int length) {
//  char p[length + 1];
//  memcpy(p, payload, length);
//  p[length] = NULL;
//  String message(p);
//  if (message == "0") {
//    digitalWrite(relay, LOW);
//  } else {
//    digitalWrite(relay, HIGH);
//  }
//  
//  Serial.write(payload, length);
//  Serial.println();
//}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME,TOKEN, "")) {
      Serial.println("Connected");
//      client.subscribe(topicSubscribe);
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
//  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
//    client.subscribe(topicSubscribe);   
    reconnect();
  }
  
  Serial.println("Publishing data to Ubidots Cloud\n");
  
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  float dummy = random(70,80);
  dtostrf(dummy, 4, 2, str_sensor);
  client.publish("/v1.6/devices/esp32/humidity-sensor",str_sensor);
  Serial.print("TOPIC: /v1.6/devices/esp32/humidity-sensor ");
  Serial.println(dummy);
  delay(1000);

  //**********************************
  dummy = random(2,10); 
  dtostrf(dummy, 4, 2, str_sensor);
  client.publish("/v1.6/devices/esp32/smoke-sensor",str_sensor);
  Serial.print("TOPIC: /v1.6/devices/esp32/smoke-sensor ");
  Serial.println(dummy);
  delay(1000);

  //**********************************
  dummy = random(55,63);
  dtostrf(dummy, 4, 2, str_sensor);
  client.publish("/v1.6/devices/esp32/soil-moisture",str_sensor);
  Serial.print("TOPIC: /v1.6/devices/esp32/soil-moisture ");
  Serial.println(dummy);
  delay(1000);

  //**********************************
  dummy = random(20,24);
  dtostrf(dummy, 4, 2, str_sensor);
  client.publish("/v1.6/devices/esp32/temperature",str_sensor);
  Serial.print("TOPIC: /v1.6/devices/esp32/temperature ");
  Serial.println(dummy);
  delay(1000);

  //**********************************
  dummy = random(4,5);
  dtostrf(dummy, 4, 2, str_sensor);
  client.publish("/v1.6/devices/esp32/soil-ph-level",str_sensor);
  Serial.print("TOPIC: /v1.6/devices/esp32/soil-ph-level ");
  Serial.println(dummy);
  delay(1000);
  
  //**********************************
  dummy = random(70,75);
  dtostrf(dummy, 4, 2, str_sensor);
  client.publish("/v1.6/devices/esp32/sun-light",str_sensor);
  Serial.print("TOPIC: /v1.6/devices/esp32/sun-light ");
  Serial.println(dummy);
  delay(1000);
 
  client.loop();
}
