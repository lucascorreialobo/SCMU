#include <esp_now.h>

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

// Global copy of slave
#define NUMSLAVES 20
SensorData sensorDataArray[NUMSLAVES];
int sensorDataCounter = 0;


bool scanedForSlaves = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void setup() {
  Serial.begin(9600);

  setup_master_connection();
  start_DHT_sensor();

}

void loop() {  
  
  ScanForSlave();
  delay(1000);
  manageSlave();
  //handle_connection();
  // send_data(get_sensor_data());
  delay(5000);
}


