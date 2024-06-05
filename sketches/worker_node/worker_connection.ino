

void setUpWifi() {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}


void disconnect_WiFi(){
  Serial.print("Disconnecting from WiFi...");
  WiFi.disconnect();
  while (WiFi.status() == WL_CONNECTED)
  {
      Serial.print(".");
      delay(300);
  }
  Serial.println("Disconnected from WiFi");
}