
#define DHT_PIN 27 //Digital pin connected to the DHT sensor
#define MQ2_PIN 14

struct SensorData {
  float temperatureC; // temperature reading in Celsius
  float temperatureF; // temperature reading in Fahrenheit
  float humidity;    // humidity reading
  float gas;
};


void setup() {
  Serial.begin(9600);

  setup_connection();
  start_DHT_sensor();

}

void loop() {  
  
  //handle_connection();
  send_data(get_sensor_data());

  delay(5000);
}


