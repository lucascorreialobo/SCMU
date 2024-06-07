const char *ssid = "yourAP";
const char *password = "yourPassword";


bool readHeaders_IsCorrectPOST(WiFiClient client, bool print = false){
  int contentLenght = 0;
  bool firstLine = true;

  while (client.connected()) {

    if (client.available()) {            // if there's bytes to read from the client,
      String line = client.readStringUntil('\n');
      Serial.println(line);

      //check if it's a POST in /coordinates
      if( firstLine && !line.startsWith("POST /coordinates")){ 
        Serial.println("Request received but wrong one");
        return false;
      }
      firstLine = false;

      //line with \r means end of headers
      if( line == "\r"){
        Serial.println("finnished reading headers");
        return true;
      }
    }

  }
  return false;
}

String ReadBody(WiFiClient client) {
  String body = "";

  if(client.connected())
    body = client.readString();
  
  return body;
}

void createRequestResponse(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  // the content of the HTTP response follows the header:
  client.print("Coordinates set.");

  // The HTTP response ends with another blank line:
  client.println();

  // close the connection:
  client.stop();
}

Coordinates StartServer(){
  WiFiServer server(80);

  String latitude   = "";
  String longitude  = "";
  bool readCoordinates = false;

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");

  while(true){
    WiFiClient client = server.available();  // listen for incoming clients

    if (client) {                            // if a new client connects,
      Serial.println("New Client.");         // print a message out in the serial port
      bool correctRequest = readHeaders_IsCorrectPOST(client);

      if(correctRequest){
        String body = ReadBody(client);
        // Serial.println(body);

        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, body);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
        } else {
          String lat = doc["latitude"];
          String longi = doc["longitude"];

          latitude = lat;
          longitude = longi;
        }

        Serial.println("Client Disconnected.");
        Serial.println("POST data: " + body); // print the POST data
        createRequestResponse(client);
        break;
      }

      createRequestResponse(client);
    }
  }

  server.close();
  return Coordinates(latitude, longitude);
}

