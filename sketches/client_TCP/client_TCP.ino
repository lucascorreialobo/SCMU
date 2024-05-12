// ESP32: TCP CLIENT + A BUTTON/SWITCH
#include <WiFi.h>

#define BUTTON_PIN 21 // ESP32 pin GPIO21 connected to button


const char* ssid = "LL";     // CHANGE TO YOUR WIFI SSID
const char* password = "LucasL00"; // CHANGE TO YOUR WIFI PASSWORD
const char* serverAddress = "192.168.201.63"; // CHANGE TO ESP32#2'S IP ADDRESS
const int serverPort = 4080;

WiFiClient TCPclient;

void setup() {
  Serial.begin(9600);

  Serial.println("ESP32: TCP CLIENT");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }
}

void loop() {

  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Reconnected to TCP server");
    } else {
      Serial.println("Failed to reconnect to TCP server");
    }
  }

  String myString = "Connected to the matrix"; // Your string
  byte byteArray[myString.length()]; // Create byte array to hold converted bytes

  // Convert string to array of bytes
  myString.getBytes(byteArray, myString.length() + 1); // +1 to include the null terminator

  TCPclient.write("Connected to the matrix");
  TCPclient.flush();
  Serial.println("- Connection to the matrix established");

  sleep(5);

  TCPclient.write("End connection to the matrix");
  TCPclient.flush();
  Serial.println("- Connection to the matrix finished");
}