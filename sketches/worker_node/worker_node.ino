#include <esp_now.h>
#include <Arduino.h>
#include "driver/rtc_io.h"
#include "soc/rtc_io_reg.h"
#include <WiFi.h>
#include "time.h"
#include <FirebaseClient.h>
#include <HTTPClient.h>
#include <math.h>


#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 25

#define BUZZZER_PIN 16

const int TIME_TO_SLEEP = 10;           /* Time ESP32 will go to sleep (in microseconds); multiplied by above conversion to achieve seconds*/
const int TIME_TO_WORK = 20 * 1000; //30
const int TIME_TO_RCV_SIGNAL = 40 * 1000;

const char* ssid     = "NOS_Internet_4FC7";
const char* password = "67827246";

String mySSID = "yourAP";
String myPassword = "yourPassword";

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



  // ulp_setup(); // it really only runs on the first ESP32 boot
  // set_fadeCycleDelay();

  setup_slave_connection();

  //Location setup cycle
  locationSetup();

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
    check_for_dangers(get_sensor_data());
    Serial.println("Signaling time is over. Entering Deep Sleep");
    Serial.flush();
    start_sleep_for(TIME_TO_SLEEP);
  }
  else{
    Serial.println("Starting Work");
    start_DHT_sensor();
  }

  previousMillis = millis();
}

void loop() {
  
  unsigned long currentMillis = millis();
  Serial.println(currentMillis - previousMillis);
  if (currentMillis - previousMillis >= TIME_TO_WORK) {
    Serial.println("Working time is over. Entering Deep Sleep");
    start_sleep_for(TIME_TO_SLEEP);
  }

  manageMaster();
  SensorData data_to_send = get_sensor_data();
  send_data(data_to_send);

  delay(2000);

  if(wasDeliverySuccessful()){
      check_for_dangers(data_to_send);
      Serial.println("Sent data successfully. Entering Deep Sleep");
      Serial.flush();
      start_sleep_for(TIME_TO_SLEEP);
    }

  // previousMillis = currentMillis;
}


