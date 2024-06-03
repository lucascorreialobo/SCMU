#include <esp_now.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"
#include <WiFi.h>

#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 14

const char* ssid     = "MEO-2B50B0";//"NOS_Internet_4FC7";
const char* password = "93ebddd008";//"67827246";

const int TIME_TO_SLEEP = 10;           /* Time ESP32 will go to sleep (in microseconds); multiplied by above conversion to achieve seconds*/
const int TIME_TO_WORK = 10 * 1000; // the duration that master is turned on
const int TIME_TO_SIGNAL = 60 * 1000;

int current_time_to = 0; 

struct SensorData {
  float temperatureC; // temperature reading in Celsius
  float temperatureF; // temperature reading in Fahrenheit
  float humidity;    // humidity reading
  float gas;
  float windSpeed;
  float rain;
  bool isSmokeDanger; 
  float FWI;
  struct tm timeinfo;
};

// Global copy of slave
#define NUMSLAVES 20
SensorData sensorDataArray[NUMSLAVES];
int sensorDataCounter = 0;


bool scanedForSlaves = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

unsigned long previousMillis = 0;
unsigned long previousSlaveScan = 0;
const unsigned long slaveScanInterval = 5000;
bool doScanSlave = true;
const unsigned long manageSlaveInterval = 1000;
bool doManageSlave = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial to start

  ulp_setup(); // it really only runs on the first ESP32 boot
  set_fadeCycleDelay();

  setup_master_connection();
  Serial.println(WiFi.localIP());
  current_time_to = TIME_TO_SIGNAL;
  
  // unsigned long currentMillis = millis();
  // bool isSleepyTime = true;

  // while(currentMillis < TIME_TO_SCAN_MASTER){
  //   ScanForSlave();
  //   if(wereSlavesFound()){
  //     isSleepyTime = false;
  //     break;
  //   }
  //   currentMillis = millis();
  // }

  // if(isSleepyTime){
  //   Serial.println("Entering in Deep Sleep");
  //   start_sleep_for(TIME_TO_SLEEP);
  // }

  bool isSleepyTime = true;
  unsigned long currentMillis = millis();
  while (currentMillis <= TIME_TO_SIGNAL) {
    ScanForSlave();
    if(wereSlavesFound()){
      // current_time_to = TIME_TO_WORK;
      isSleepyTime = false;
      break;
    }
    currentMillis = millis();
  }
  
  if(isSleepyTime){
    Serial.println("Signaling time is over. Entering Deep Sleep");
    start_sleep_for(TIME_TO_SLEEP);
  }
  else{
    start_DHT_sensor();
    FirebaseSetUp();
  }
  
}

void loop() {  

  unsigned long currentMillis = millis();

    // Check if it's time to sleep
  if (currentMillis - previousMillis <= TIME_TO_WORK) {
    printSensorData();
    delay(5000);
    Serial.println("Working time is over. Entering Deep Sleep");
    start_sleep_for(TIME_TO_SLEEP);
  }

  // ScanForSlave();
  manageSlave();
  // if(wereSlavesFound()){
  //   current_time_to = TIME_TO_WORK;
  //   startAsyncFirebase();
  //   firebase_code();
  //   manageSlave();
  // }
  delay(1000);
  // if (doScanSlave && currentMillis - previousSlaveScan >= slaveScanInterval) {
  //   ScanForSlave();
  //   previousSlaveScan = currentMillis;
  //   doScanSlave = false;
  //   doManageSlave = true;
  // }

  // if(doManageSlave && currentMillis - previousSlaveScan >= manageSlaveInterval ){
  //   manageSlave();
  //   previousSlaveScan = currentMillis;
  //   doScanSlave = true;
  //   doManageSlave = false;
  // }

  previousMillis = currentMillis;
}

void printSensorData() {
    for (int i = 0; i < sensorDataCounter; i++) {
        printf("Sensor %d:\n", i + 1);
        printf("Temperature (C): %.2f\n", sensorDataArray[i].temperatureC);
        printf("Temperature (F): %.2f\n", sensorDataArray[i].temperatureF);
        printf("Humidity: %.2f%%\n", sensorDataArray[i].humidity);
        printf("Gas: %.2f\n", sensorDataArray[i].gas);
        printf("Wind Speed: %.2f m/s\n", sensorDataArray[i].windSpeed);
        printf("Rain: %.2f mm\n", sensorDataArray[i].rain);
        printf("Smoke Danger: %s\n", sensorDataArray[i].isSmokeDanger ? "Yes" : "No");
        printf("Fire Weather Index (FWI): %.2f\n", sensorDataArray[i].FWI);
        
        // Convert timeinfo to human-readable format
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &sensorDataArray[i].timeinfo);
        printf("Timestamp: %s\n", buffer);

        printf("\n");
    }
}


