#include <ESP8266WiFi.h> 

#include "Adafruit_MQTT.h" 

#include "Adafruit_MQTT_Client.h" 

 

#define WLAN_SSID "Durbar"  // replace with your wifi ssid and wpa2 key 

#define WLAN_PASS "Durbar@7777" 

 
 

#define AIO_SERVER "io.adafruit.com" 

#define AIO_SERVERPORT 1883 

 
 

#define AIO_USERNAME "surya111"                   // Your Adafruit IO Username 

#define AIO_KEY "aio_QxZD15XUsTroPSSwcxGIW0Piqj3d"  // Adafruit IO AIO key 

 
 

const int sensorPin = 4;  

int sensorState = 0; 

int lastState = 0; 

 
 
 

WiFiClient client; 

 
 
 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 

 
 

Adafruit_MQTT_Publish soilMoisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/Feeds/MoistureValue"); 

 
 
 
 
 
 

void setup() { 

 
 

  Serial.begin(9600); 

  pinMode(A0, INPUT); 

 

  Serial.println("Connecting to "); 

  Serial.println(WLAN_SSID); 

 
 

  WiFi.begin(WLAN_SSID, WLAN_PASS); 

 
 

  while (WiFi.status() != WL_CONNECTED) { 

    delay(500); 

    Serial.print("."); 

  } 

  Serial.println(""); 

  Serial.println("WiFi connected"); 

  Serial.println("IP address: "); 

  Serial.println(WiFi.localIP()); 

} 

 
 
 

void loop() { 

  MQTT_connect(); 

 

  sensorState = analogRead(A0); 

  Serial.println(sensorState); 

 

  if (sensorState == 1 && lastState == 0) { 

    Serial.println("needs water, send notification"); 

    lastState = 1; 

    delay(500); 

    //send notification 

 

  } else if (sensorState == 1 && lastState == 1) { 

    //do nothing, has not been watered yet 

    Serial.println("has not been watered yet"); 

    delay(500); 

  } else { 

    //st 

    Serial.println("does not need water"); 

    lastState = 0; 

    delay(500); 

  } 

 

  soilMoisture.publish(sensorState); 

  delay(1000); 

} 

 
 
 
 
 
 
 

void MQTT_connect() { 

  int8_t ret; 

 
 

  // Stop if already connected. 

  if (mqtt.connected()) { 

    return; 

  } 

 
 

  Serial.print("Connecting to MQTT... "); 

 
 

  uint8_t retries = 3; 

 
 

  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected 

    Serial.println(mqtt.connectErrorString(ret)); 

    Serial.println("Retrying MQTT connection in 5 seconds..."); 

    mqtt.disconnect(); 

    delay(5000);  // wait 5 seconds 

    retries--; 

    if (retries == 0) { 

      // basically die and wait for WDT to reset me 

      while (1) 

        ; 

    } 

  } 

  Serial.println("MQTT Connected!"); 

} 

 
