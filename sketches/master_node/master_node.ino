#include <esp_now.h>
#include "driver/rtc_io.h"
#include <WiFi.h>
#include "time.h"
#include <FirebaseClient.h>
#include <HTTPClient.h>

#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 14

#define LED_BUILTIN 17
#define BUZZZER_PIN  16 // ESP32 pin GPIO4 connected to piezo buzzer

String mySSID = "yourAP";
String myPassword = "yourPassword";

const char* ssid     = "LL";//MEO-F59510
const char* password = "LucasL00";//casadoslobos
//const char* ssid     = "NOS_Internet_4FC7";//"NOS_Internet_4FC7";
//const char* password = "67827246";//"67827246";

const int TIME_TO_SLEEP = 10;           /* Time ESP32 will go to sleep (in microseconds); multiplied by above conversion to achieve seconds*/
const int TIME_TO_WORK = 20 * 1000; // the duration that master is turned on
const int TIME_TO_SIGNAL = 60 * 1000;//10 * 1000;

String forestID = "Floresta de Monsanto";


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
  pinMode(BUZZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
  //Location setup cycle
  locationSetup();
  checkActuators(20);
  
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
    delay(5000);
    Serial.println("Signaling time is over. Entering Deep Sleep");
    Serial.flush();
    start_sleep_for(TIME_TO_SLEEP);
  }
  else{
    Serial.println("Starting Work");
  }
  previousMillis = millis();
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

  //previousMillis = currentMillis;
}

void printSensorData() {
  Serial.println("******************* Printing collected sensor Data ******************************");
    for (int i = 0; i < sensorDataCounter; i++) {
        Serial.printf("Sensor %d:\n", i + 1);
        Serial.printf("MacAddress: "); printMacAddress(sensorDataArray[i].macAddress);
        Serial.printf("Location:\n\tLatitude: %s\n\tLongitude: %s.\n", sensorDataArray[i].coordinates.latitude, sensorDataArray[i].coordinates.longitude);
        Serial.printf("Temperature (C): %.2f\n", sensorDataArray[i].temperatureC);
        Serial.printf("Temperature (F): %.2f\n", sensorDataArray[i].temperatureF);
        Serial.printf("Humidity: %.2f%%\n", sensorDataArray[i].humidity);
        Serial.printf("Gas: %.2f\n", sensorDataArray[i].gas);
        Serial.printf("Wind Speed: %.2f m/s\n", sensorDataArray[i].windSpeed);
        Serial.printf("Rain: %.2f mm\n", sensorDataArray[i].rain);
        Serial.printf("Smoke Danger: %s\n", sensorDataArray[i].isSmokeDanger ? "Yes" : "No");
        Serial.printf("Fire Weather Index (FWI): %.2f\n", sensorDataArray[i].local_FWI);
        Serial.
        printf("\n");
    }
  Serial.println("******************* END Printing collected sensor Data ******************************");
}
