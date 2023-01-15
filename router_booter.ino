/*
Title: Router-Booter
Description: 
This program allows an ESP8266 to pings an address, 
if the address can't be reached, 
the relay turns off and turns back on. The cycle continues,
a number of attempts can be set before a timeout of 1 hour takes place.
Author: Marco A. Barreto - marcoagbarreto
Version: 15-Jan-2023
*/

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>   //ESP8266Ping found here: https://github.com/dancol90/ESP8266Ping

const char* ssid = "your_ssid";
const char* password = "your_password";

const char* remote_host = "www.google.com";

// ESP-01/01s Relay V4.0 pins // https://github.com/IOT-MCU/ESP-01S-Relay-v4.0
int relayPin = 0; // Pin for the relay
int ledPin = 2; // Pin for the status LED
// -------------------

// Declaring variables
int reboots = 0;
int retries = 5;
// -------------------

// Delays
int delayBoot = 60000; // 60 seconds delay
int delayPing = 5000; // 5 seconds delay
int timeout = 3600000; // 1 hour delay
// -------------------

void setup() {
  Serial.begin(115200);

  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, LOW); // relay on
  digitalWrite(ledPin, LOW); // LED on

  delay(delayBoot); // 60 seconds delay

  startWifi(); // Connect Wifi network
}

void loop() {
  int pingStatus = false;
  int attempts = 0;
  while(attempts < retries){
    pingStatus = Ping.ping(remote_host);
    if (pingStatus) {
      Serial.print("Successfully pinged: ");
      Serial.println(remote_host);
      reboots = 0;
      attempts = 0;
      break;
    }
    attempts++;
    delay(delayPing); // delay of 5 second before next attempt
  }
  if(attempts == retries){
    digitalWrite(relayPin, HIGH); // relay off
    digitalWrite(ledPin, HIGH);   // LED off
    Serial.println("Rebooting");
    reboots++;
    if(reboots >= retries){
      Serial.println("Timeout: Waiting for 1 hour before next attempt");
      delay(timeout); // 1 hour delay
      reboots = 0;
    }
    else{
      delay(delayBoot); // 60 seconds delay
      digitalWrite(relayPin, LOW); // relay on
      digitalWrite(ledPin, LOW); // LED on
    }
  }
}

void startWifi(){
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("WiFi connected with ip: ");  
  Serial.println(WiFi.localIP());
}

