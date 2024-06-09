#include <esp_now.h>
#include <Arduino.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"
#include "soc/rtc_io_reg.h"
#include <WiFi.h>
#include "time.h"
#include <FirebaseClient.h>
#include <HTTPClient.h>
#include <math.h>


#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 26

const int TIME_TO_SLEEP = 10;           /* Time ESP32 will go to sleep (in microseconds); multiplied by above conversion to achieve seconds*/
const int TIME_TO_WORK = 20 * 1000; //30
const int TIME_TO_RCV_SIGNAL = 40 * 1000;

const char* ssid     = "NOS_Internet_4FC7";
const char* password = "67827246";


struct Coordinates {
  String latitude;
  String longitude;
  
  // Default constructor
  Coordinates() : latitude(""), longitude("") {}

  Coordinates(String lat, String longi): latitude(lat), longitude(longi) {}
};

struct SensorData {
  uint8_t macAddress[6]; // MAC address field
  Coordinates coordinates; 
  float temperatureC; // temperature reading in Celsius
  float temperatureF; // temperature reading in Fahrenheit
  float humidity;    // humidity reading
  float gas;
  float windSpeed;
  float rain;
  bool isSmokeDanger; 
  float local_FWI;
};


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial to start


  //Location setup cycle
  locationSetup();

  // ulp_setup(); // it really only runs on the first ESP32 boot
  // set_fadeCycleDelay();

  setup_slave_connection();
  // start_DHT_sensor();


  unsigned long currentMillis = millis();
  bool isSleepyTime = true;

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
    Serial.flush();
    start_sleep_for(TIME_TO_SLEEP);
  }
  else{
    Serial.println("Starting Work");
    start_DHT_sensor();
  }
}

void loop() {
  
  unsigned long currentMillis = millis();
  Serial.print("SHIIIIIIIIIIIIIT: ");
  Serial.println(currentMillis - previousMillis);
  Serial.print("TIME TO WORK: ");
  Serial.println(TIME_TO_WORK);
  if (currentMillis - previousMillis >= TIME_TO_WORK) {
    Serial.println("Working time is over. Entering Deep Sleep");
    start_sleep_for(TIME_TO_SLEEP);
  }

  manageMaster();
  send_data(get_sensor_data());

  delay(2000);

  if(wasDeliverySuccessful()){
      Serial.println("Sent data successfully. Entering Deep Sleep");
      Serial.flush();
      start_sleep_for(TIME_TO_SLEEP);
    }

  previousMillis = currentMillis;
}


