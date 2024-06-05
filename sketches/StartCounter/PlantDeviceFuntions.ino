const char *ssid = "yourAP";
const char *password = "yourPassword";


int readHeadersGetContentLenght(WiFiClient client, bool print = false){
  String currentLine = "";
  int contentLenght = 0;

  while (client.connected()) {

    if (client.available()) {            // if there's bytes to read from the client,
      char c = client.read();            // read a byte, then

      if(print)
        Serial.write(c);                   // print it out the serial monitor

      if (c == '\n') {                   // if the byte is a newline character

        if(currentLine.startsWith("Content-Length: ")) {
          String s = currentLine.substring(16);
          contentLenght = s.toInt();
        }

        if (currentLine.length() == 0) {
          return contentLenght;
        }
        currentLine = "";

      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }

  }
  return contentLenght;
}

String ReadBody(WiFiClient client, int contentLenght) {
  String body = "";
  for(int i = 0; i < contentLenght; i++){
    if(!client.connected()){ 
      break;
    }

    char c = client.read();
    //Serial.write(c);
    body += c;
  }
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
        String longi = doc["longitude"];

        latitude = lat;
        longitude = longi;
      }
    

      createRequestResponse(client);

      Serial.println("Client Disconnected.");
      Serial.println("POST data: " + body); // print the POST data

      server.close();
      return Coordinates(latitude, longitude);
    }
  }
  
}

