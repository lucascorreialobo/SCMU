#include <esp_now.h>
#include <Arduino.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"

#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 26

const int TIME_TO_SLEEP = 10;           /* Time ESP32 will go to sleep (in microseconds); multiplied by above conversion to achieve seconds*/
const int TIME_TO_WORK = 10 * 1000; //30
const int TIME_TO_RCV_SIGNAL = 40 * 1000;

const char* ssid     = "NOS_Internet_4FC7";
const char* password = "67827246";


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

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial to start

  ulp_setup(); // it really only runs on the first ESP32 boot
  set_fadeCycleDelay();

  setup_slave_connection();
  // start_DHT_sensor();


  unsigned long currentMillis = millis();
  bool isSleepyTime = true;

  // while(currentMillis - previousMillis >= TIME_TO_WORK){
  //   ScanForMaster();
  //   if(wasMasterFound()){
  //     isSleepyTime = false;
  //     break;
  //   }
  //   previousMillis = currentMillis;
  // }

  // if(isSleepyTime){
  //   Serial.println("Entering in Deep Sleep");
  //   start_sleep_for(TIME_TO_SLEEP);
  // }
  // From this point on, no code is executed in DEEP SLEEP mode

  while (currentMillis <= TIME_TO_RCV_SIGNAL) {
    ScanForMaster();
    if(wasMasterFound()){
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
  }
}

void loop() {
  
  unsigned long currentMillis = millis();
  if (wasDeliverySuccessful() || currentMillis - previousMillis <= TIME_TO_WORK) {
    Serial.println("Working time is over. Entering Deep Sleep");
    start_sleep_for(TIME_TO_SLEEP);
  }

  //handle_connection();
  // ScanForMaster();
  manageMaster();
  send_data(get_sensor_data());
  delay(1000);


  // unsigned long startTime = millis();
  // bool isSleepyTime = true;
  // while (millis() - startTime < TIME_TO_SCAN_MASTER*1000) { // run while loop for 5000 milliseconds
  //   ScanForMaster();
  //   if (wasMasterFound()) {
  //     isSleepyTime = false;
  //     break;
  //   }
  // }

  // if (isSleepyTime) {
  //   Serial.println("Entering in Deep Sleep");
  //   start_sleep_for(TIME_TO_SLEEP);
  // }

  previousMillis = currentMillis;
}


