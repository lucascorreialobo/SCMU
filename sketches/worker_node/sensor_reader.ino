struct SensorData sensor_data;

struct SensorData get_sensor_data(){
  DHTData dht = get_DHT_values();
  int gasValue = get_MQ2_values();

  //Set dht sensor values
  sensor_data.temperatureC = dht.temperatureC;
  sensor_data.temperatureF = dht.temperatureF;
  sensor_data.humidity = dht.humidity;
  sensor_data.gas = gasValue;

  return sensor_data;
}

