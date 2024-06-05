/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>
#include <ArduinoJson.h>


// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);

//function prototypes
int readHeadersGetContentLenght(WiFiClient client, bool print = false);
String ReadBody(WiFiClient client, int contentLenght);
void createRequestResponse(WiFiClient client);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();  // listen for incoming clients

  String latitude = "";

  if (client) {                            // if a new client connects,
    Serial.println("New Client.");         // print a message out in the serial port
    int contentLenght = readHeadersGetContentLenght(client);


    String body = ReadBody(client, contentLenght);
    // Serial.println(body);

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, body);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    } else {
      String lat = doc["latitude"];
      latitude = lat;
    }
  

    createRequestResponse(client);

    Serial.println("Client Disconnected.");
    Serial.println("POST data: " + body); // print the POST data
  }

  
}

