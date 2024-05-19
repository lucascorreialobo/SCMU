#include <esp_now.h>
#include <Arduino.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"

#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 14

const char* ssid     = "NOS_Internet_4FC7";
const char* password = "67827246";


struct SensorData {
  float temperatureC; // temperature reading in Celsius
  float temperatureF; // temperature reading in Fahrenheit
  float humidity;    // humidity reading
  float gas;
  struct tm timeinfo;
};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial to start

  ulp_setup(); // it really only runs on the first ESP32 boot
  set_fadeCycleDelay();

  setup_slave_connection();
  start_DHT_sensor();

  ScanForMaster();
  delay(1000);
  manageMaster();
  send_data(get_sensor_data());
  delay(5000);

  Serial.println("Entering in Deep Sleep");
  start_sleep();
  // From this point on, no code is executed in DEEP SLEEP mode
}

void loop() {  
  
  //handle_connection();
  ScanForMaster();
  delay(1000);
  manageMaster();
  send_data(get_sensor_data());
  delay(5000);
}


