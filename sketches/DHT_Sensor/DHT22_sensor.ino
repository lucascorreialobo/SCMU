
//Import required libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>

//code
#define DHTPIN 27 //Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22

DHT dht_sensor(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  // pinMode(DHTPIN, INPUT);
  dht_sensor.begin();
}

void loop() {
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

