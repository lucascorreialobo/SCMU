#include <WiFi.h>
#include <FirebaseClient.h>

#define WIFI_SSID "LL"
#define WIFI_PASSWORD "LucasL00"



bool taskComplete = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setUpWifi();

  FirebaseSetUp();


}

void loop() {
  // put your main code here, to run repeatedly:
  startAsyncFirebase();

  firebase_code();

}
