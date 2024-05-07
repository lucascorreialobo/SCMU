/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

// ESP32 #2: TCP SERVER + AN LED
#include <WiFi.h>

#define LED_PIN  2 // ESP32 built-in led
#define SERVER_PORT 4080

const char *ssid = "LL";  
const char *password = "LucasL00";

WiFiServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(9600);
  //pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  //Serial.println("ESP32 #2: TCP SERVER + AN LED");
  //WiFi.disconnect();
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Connected to WiFi");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Start listening for a TCP client (from ESP32 #1)
  TCPserver.begin();
}

void loop() {
  // Wait for a TCP client from ESP32 #1:
  WiFiClient client = TCPserver.available();

  

  if (client) {
    // Read the command from the TCP client:
    //char * command;
    //char command = client.read();
    String response = client.readStringUntil('\n');
    //client.readBytes(command, 3);
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(response);


    client.stop();
  }
}