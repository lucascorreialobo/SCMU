// Replace with your Firebase Cloud Messaging server key
const char *serverKey = "BIEKPbaB0wshObkmhUaToPobycVjNhjzqtIXYZcedNlJpbhK0ha8gBkS-u-DBP2iqNXtrW71fLIFU_m6V1caQG8";

void sendNotification(const char *title, const char *body) {
  setUpWifi();
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://fcm.googleapis.com/fcm/send");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(serverKey));

    String payload = "{\"to\":\"/topics/sensor_alerts\",\"notification\":{\"title\":\"" + String(title) + "\",\"body\":\"" + String(body) + "\"}}";
    
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  disconnect_WiFi();
}
