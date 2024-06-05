#include <Preferences.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>
#include <ArduinoJson.h>


Preferences preferences;

struct Coordinates {
  String latitude;
  String longitude;

  Coordinates(String lat, String longi): latitude(lat), longitude(longi) {}


};


void setup() {
  Serial.begin(9600);
  Serial.println();

  
  // We will open storage in RW-mode (second parameter has to be false).
  preferences.begin("Averno", false);

  if(preferences.isKey("latitude") && preferences.isKey("longitude")) {
    Serial.printf("The location for this device is already set at: (%s;%s)\n", preferences.getString("latitude"), preferences.getString("longitude"));
  }

  // Remove all preferences under the opened namespace
  //preferences.clear();

  // Or remove the counter key only
  //preferences.remove("counter");

  bool skipStartup = preferences.getBool("Skip Startup", false);

  if(!skipStartup){
    startupCycle();
  }

  preferences.putBool("Skip Startup", false); //by default set skip startup to false, if going to forced sleep set to true

  // Close the Preferences
  preferences.end();

}

void loop() {


  
}


void startupCycle() {
  Coordinates location = StartServer();

  preferences.putString("latitude", location.latitude);
  preferences.putString("longitude", location.longitude);

  Serial.println("Location setup finilized.");
}