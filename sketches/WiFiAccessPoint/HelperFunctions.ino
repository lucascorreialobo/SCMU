
int readHeadersGetContentLenght(WiFiClient client, bool print){
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

