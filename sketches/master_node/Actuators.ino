unsigned long buzzerStart = 0;
unsigned long ledStart = 0;


//void dealActuators(){
//  unsigned long currentTime = millis();
//  dealBuzzer(buzzerStart, currentTime);
//  dealLED(buzzerStart, currentTime);
//}



void checkActuators(int tryForSeconds){
  WiFiServer server(80);

  //if (!WiFi.softAP(mySSID, myPassword)) {
  //  Serial.print("Soft AP creation failed.");
  //  while (1);
  //}
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");

  int currentTime = millis();
  int timeToExit = currentTime + (tryForSeconds*1000);

  while(currentTime < timeToExit){
    WiFiClient client = server.available();  // listen for incoming clients

    if (client) {                            // if a new client connects,
      Serial.println("New Client.");         // print a message out in the serial port
      bool expectedRequest = readHeaders(client, "GET /buzzer");
      Serial.println("Inside");
      if(expectedRequest){
        playTune();
        dealLED();
        break;
      }
    }

    //will do the necessary changes to the actuators
    //dealActuators();

    currentTime = millis();
  }
  if(currentTime >= timeToExit){
    Serial.println("Actuator activation timed out, continuing with program.");
  }
}

/**
*   returns true if request has the expected URL
*/
bool readHeaders(WiFiClient client, String expectedURL){  //expectedURL examples: "GET /buzzer", "POST /coordinates", ...
  bool firstLine = true;

  while (client.connected()) {

    if (client.available()) {            // if there's bytes to read from the client,
      String line = client.readStringUntil('\n');

      if( firstLine && !line.startsWith(expectedURL)){ 
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
}

