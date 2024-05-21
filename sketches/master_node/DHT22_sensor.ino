//Import required libraries
#include <DHT.h>

struct DHTData {
  float temperatureC; // temperature reading in Celsius
  float temperatureF; // temperature reading in Fahrenheit
  float humidity;    // humidity reading
} dht_data;

//code
#define DHTTYPE DHT22 // DHT 22

DHT dht_sensor(DHT_PIN, DHTTYPE);

void start_DHT_sensor(){
  dht_sensor.begin();
  Serial.println("DHT is on!");
}

float read_DHT_temperature(bool isFahrenheit = false){
  float temp = isFahrenheit ? dht_sensor.readTemperature(true) : dht_sensor.readTemperature();
  if(isnan(temp)){
    Serial.println("Failed to read temperature from DHT sensor!");
  }
  return temp;
}

float read_DHT_humidity(){
  float hum = dht_sensor.readHumidity();
  if(isnan(hum)){
    Serial.println("Failed to read humidity from DHT sensor!");
  }
  return hum;
}

struct DHTData get_DHT_values(){

  // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  dht_data.temperatureC = tempC;
  dht_data.temperatureF = tempF;
  dht_data.humidity = humi;
   
	return dht_data;
}

/*
void read_DHT_values(){

  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");
  }

  // wait a 2 seconds between readings
  delay(2000);
  
}
*/

