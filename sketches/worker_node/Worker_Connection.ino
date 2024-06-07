// Global copy of Master
#define NUMMASTER 1
esp_now_peer_info_t master[NUMMASTER] = {};
int MasterCnt = 0;

bool was_delivery_sucess = false;

#define CHANNEL 1
#define PRINTSCANRESULTS 0

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

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


// config AP SSID
void configSlaveDeviceAP() {
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

// Scan for Master in AP mode
void ScanForMaster() {
  int8_t scanResults = WiFi.scanNetworks();
  //reset master
  memset(master, 0, sizeof(master));
  MasterCnt = 0;
  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
      }
      delay(10);
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Master") == 0) {
        // SSID of interest
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];

        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            master[MasterCnt].peer_addr[ii] = (uint8_t) mac[ii];
          }
        }
        master[MasterCnt].channel = CHANNEL; // pick a channel
        master[MasterCnt].encrypt = 0; // no encryption
        MasterCnt++;
      }
    }
  }

  if (MasterCnt > 0) {
    Serial.print(MasterCnt); Serial.println(" Master found, processing..");
  } else {
    Serial.println("No Master Found, trying again.");
  }

  // clean up ram
  WiFi.scanDelete();
}

bool wasMasterFound(){
  return MasterCnt > 0;
}

// Check if the slave is already paired with the master.
// If not, pair the slave with master
void manageMaster() {
  if (MasterCnt > 0) {
    for (int i = 0; i < MasterCnt; i++) {
      Serial.print("Processing: ");
      for (int ii = 0; ii < 6; ++ii ) {
        Serial.print((uint8_t) master[i].peer_addr[ii], HEX);
        if (ii != 5) Serial.print(":");
      }
      Serial.print(" Status: ");
      // check if the peer exists
      bool exists = esp_now_is_peer_exist(master[i].peer_addr);
      if (exists) {
        // Slave already paired.
        Serial.println("Already Paired");
      } else {
        // Slave not paired, attempt pair
        esp_err_t addStatus = esp_now_add_peer(&master[i]);
        if (addStatus == ESP_OK) {
          // Pair success
          Serial.println("Pair success");
        } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
          // How did we get so far!!
          Serial.println("ESPNOW Not Init");
        } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
          Serial.println("Add Peer - Invalid Argument");
        } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
          Serial.println("Peer list full");
        } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
          Serial.println("Out of memory");
        } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
          Serial.println("Peer Exists");
        } else {
          Serial.println("Not sure what happened");
        }
        delay(100);
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Master found to process");
  }
}

void setup_slave_connection(){
  WiFi.mode(WIFI_AP);

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected.");
  
  // Init and get the time
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // setup_Time();

  // configure device AP mode
  configSlaveDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();

  esp_now_register_send_cb(OnDataSent);
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);

  //receive master Mac Address
}

// callback when data is sent from Slave to Master
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  if(status == ESP_NOW_SEND_SUCCESS){
    was_delivery_sucess = true;
    Serial.println("Delivery Success");
  }
  else{
    Serial.println("Delivery Fail");
  }
}

// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  // Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  // Serial.println("");
}

void getMacAddress(uint8_t *mac) {
  WiFi.macAddress(mac);
}


uint8_t test_data = 0;
void send_data(SensorData data) {
  test_data++;
  getMacAddress(data.macAddress);

  // data.coordinates = getPreferencesCoordinates();

  for (int i = 0; i < MasterCnt; i++) {
    const uint8_t *peer_addr = master[i].peer_addr;
    Serial.print("Sending to peer addr: ");
    Serial.println(*peer_addr);
    if (i == 0) {
      Serial.println("Sending: sensorial data");
    }
    esp_err_t result = esp_now_send(peer_addr, (uint8_t *) &data, sizeof(data));
    Serial.println("Send Status: ");
    if (result == ESP_OK) {
      Serial.println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
      // How did we get so far!!
      Serial.println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
      Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
      Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
      Serial.println("Peer not found.");
    } else {
      Serial.println("Not sure what happened");
    }
    delay(100);
  }
}

bool wasDeliverySuccessful(){
  return was_delivery_sucess;
}

