#include <esp_now.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"
#include <WiFi.h>
#include "time.h"
#include <FirebaseClient.h>
#include <HTTPClient.h>

#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 14

const char* ssid     = "MEO-F59510";
const char* password = "casadoslobos";
// const char* ssid     = "NOS_Internet_4FC7";//"NOS_Internet_4FC7";
// const char* password = "67827246";//"67827246";

const int TIME_TO_SLEEP = 10;           /* Time ESP32 will go to sleep (in microseconds); multiplied by above conversion to achieve seconds*/
const int TIME_TO_WORK = 20 * 1000; // the duration that master is turned on
const int TIME_TO_SIGNAL = 10 * 1000;//60 * 1000;

String forestID = "Floresta da FCT";


struct SensorData {
  uint8_t macAddress[6]; // MAC address field
  float temperatureC; // temperature reading in Celsius
  float temperatureF; // temperature reading in Fahrenheit
  float humidity;    // humidity reading
  float gas;
  float windSpeed;
  float rain;
  bool isSmokeDanger; 
  float local_FWI;
};

struct Coordinates {
  String latitude;
  String longitude;

  Coordinates(String lat, String longi): latitude(lat), longitude(longi) {}
};

#define NUMSLAVES 20

SensorData sensorDataArray[NUMSLAVES];
int sensorDataCounter = 0;

int SlaveCnt = 0;


bool scanedForSlaves = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

unsigned long previousMillis = 0;
unsigned long previousSlaveScan = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial to start

  //Location setup cycle
  locationSetup();

  ulp_setup(); // it really only runs on the first ESP32 boot
  set_fadeCycleDelay();

  setup_master_connection();
  Serial.println(WiFi.localIP());

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
  start_DHT_sensor();
  if(isSleepyTime){
    sensorDataArray[sensorDataCounter++] = get_sensor_data();
    printSensorData();
    delay(5000);
    //send data to fire base
    firebase_code();

    Serial.println("Signaling time is over. Entering Deep Sleep");
    Serial.flush();
    start_sleep_for(TIME_TO_SLEEP);
  }
  else{
    Serial.println("Starting Work");
  }
  
}

void loop() {  

  unsigned long currentMillis = millis();

    // Check if it's time to sleep
  if (currentMillis - previousMillis >= TIME_TO_WORK || sensorDataCounter >= SlaveCnt) {
    sensorDataArray[sensorDataCounter++] = get_sensor_data();
    Serial.println("********** LAST PRINT ARR, BEFORE SLEEP ********");
    printSensorData();
    delay(5000);
    //send data to fire base
    firebase_code();

    delay(5000);
    Serial.println("Working time is over. Entering Deep Sleep");
    Serial.flush();
    start_sleep_for(TIME_TO_SLEEP);

  }

  // ScanForSlave();
  manageSlave();

  delay(1000);

  previousMillis = currentMillis;
}

void printSensorData() {
  Serial.println("******************* Printing collected sensor Data ******************************");
    for (int i = 0; i < sensorDataCounter; i++) {
        printf("Sensor %d:\n", i + 1);
        printf("MacAddress: "); printMacAddress(sensorDataArray[i].macAddress);
        printf("Temperature (C): %.2f\n", sensorDataArray[i].temperatureC);
        printf("Temperature (F): %.2f\n", sensorDataArray[i].temperatureF);
        printf("Humidity: %.2f%%\n", sensorDataArray[i].humidity);
        printf("Gas: %.2f\n", sensorDataArray[i].gas);
        printf("Wind Speed: %.2f m/s\n", sensorDataArray[i].windSpeed);
        printf("Rain: %.2f mm\n", sensorDataArray[i].rain);
        printf("Smoke Danger: %s\n", sensorDataArray[i].isSmokeDanger ? "Yes" : "No");
        printf("Fire Weather Index (FWI): %.2f\n", sensorDataArray[i].local_FWI);
        
        printf("\n");
    }
  Serial.println("******************* END Printing collected sensor Data ******************************");
}

