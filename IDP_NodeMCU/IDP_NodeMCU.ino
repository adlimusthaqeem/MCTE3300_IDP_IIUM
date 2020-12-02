/* Sending Sensor Data to Firebase Database by CircuitDigest(www.circuitdigest.com) */
// ----------------------------------Temperature Library Setup---------------------------------- /
#include "DHT.h"
#define DHTPIN 2      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float humidity;    //to store humidity data
float temperature; //to store temperature data

#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library                                                         // dht11 temperature and humidity sensor library
#define FIREBASE_HOST "testidp2-fa7e9.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "G8gaZG5oMtvfOZ8tWBLFUjKtFJ5x9zDGB3HZPgUn"            // the secret key generated from firebase

#define WIFI_SSID "Hanafi0808@unifi"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "JebatTeja95"                                    //password of wifi ssid
String test;

void readDHT()
{
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  humidity = h;
  temperature = t;
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase

}

void loop() {
  String fireTemp = String(temperature) + String("Â°C");
  delay(4000);
  Firebase.pushString("/DHT11/Temperature", fireTemp);                                //setup path and send readings
  if (Serial.available())
  {
    test = String (Serial.read());
    Firebase.pushString("/Command/Test Code", test);
  }

  byte n = SUART.available(); //n != 0 means a character has arrived
  if (n != 0)
  {
    String x = String (SUART.read());
    Firebase.pushString("/Command/Test Code", x);     
  }
}

