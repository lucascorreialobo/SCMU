#define SMOKE_THRESHOLD 300

// Initialize Fire Weather Index components (use starting values or load from storage)
float FFMC = 85.0; // Example starting value
float DMC = 6.0;  // Example starting value
float DC = 15.0;  // Example starting value
float ISI = 5.0;  // Example starting value
float BUI = 20.0; // Example starting value

float wind_speed_val = 5; // since we don't have such sensor we use constant
float rain_val = 0; // since we don't have such sensor we use constant

struct SensorData sensor_data;

struct SensorData get_sensor_data(){
  DHTData dht = get_DHT_values();
  int gasValue = get_MQ2_values();

  //Set dht sensor values
  sensor_data.temperatureC = dht.temperatureC;
  sensor_data.temperatureF = dht.temperatureF;
  sensor_data.humidity = dht.humidity;
  sensor_data.gas = gasValue;
  sensor_data.windSpeed = wind_speed_val;
  sensor_data.rain = rain_val;
  sensor_data.timeinfo = printLocalTime();
  sensor_data.FWI = fwi_calc();
  sensor_data.isSmokeDanger = isDangerSmoke();

  Serial.println("Sensor Data:");
  Serial.print("Temp: "); Serial.println(sensor_data.temperatureC);
  Serial.print("Hum: "); Serial.println(sensor_data.humidity);
  Serial.print("Smoke/Gas: "); Serial.println(sensor_data.gas);
  return sensor_data;
}

bool isDangerSmoke(){
   // Include logic to determine if smoke levels are indicative of a fire
  bool possible_fire = sensor_data.gas > SMOKE_THRESHOLD; // Define a suitable threshold based on calibration

  // Print smoke level and fire warning
  Serial.print("Smoke Level: ");
  Serial.print(sensor_data.gas);
  Serial.println(" ppm");
  if (possible_fire) {
    Serial.println("Warning: High smoke levels detected!");
  }

  return possible_fire;
}


float fwi_calc(){
  // Read sensors
  float temperature = sensor_data.temperatureC;
  float humidity = sensor_data.humidity;
  float windSpeed = sensor_data.windSpeed;
  float rain = sensor_data.rain;

  // Calculate FWI components (simplified)
  // This requires proper formulas based on the Canadian Forest Fire Weather Index System
  FFMC = calculateFFMC(FFMC, temperature, humidity, windSpeed);
  DMC = calculateDMC(DMC, temperature, humidity, rain);
  DC = calculateDC(DC, temperature, rain);
  ISI = calculateISI(ISI, windSpeed);
  BUI = calculateBUI(DMC, DC);

  // Calculate the FWI based on the components
  float FWI = calculateFWI(ISI, BUI);

  // Output the result
  Serial.print("FWI: ");
  Serial.println(FWI);
  return FWI;
}

float calculateFFMC(float prevFFMC, float temp, float rh, float wind) {
  float mo = 147.2 * (101.0 - prevFFMC) / (59.5 + prevFFMC);
  float Ed = 0.942 * pow(rh, 0.679) + 11 * exp((rh - 100) / 10.0) + 0.18 * (21.1 - temp) * (1 - 1 / exp(0.115 * rh));
  float Ew = 0.618 * pow(rh, 0.753) + 10 * exp((rh - 100) / 10.0) + 0.18 * (21.1 - temp) * (1 - 1 / exp(0.115 * rh));
  
  float k1 = 0.424 * (1 - pow((100 - rh) / 100, 1.7)) + 0.0694 * sqrt(wind) * (1 - pow((100 - rh) / 100, 8));
  float k2 = 0.424 * (1 - pow((rh) / 100, 1.7)) + 0.0694 * sqrt(wind) * (1 - pow((rh) / 100, 8));
  
  float m = mo + k1 * (Ew - mo) + k2 * (Ed - Ew);
  return 59.5 * (250.0 - m) / (147.2 + m);
}

float calculateDMC(float prevDMC, float temp, float rh, float rain) {
  float re = (rain > 1.5) ? (rain - 1.5) : 0;
  float Mo = 20 + exp(5.6348 - prevDMC / 43.43);
  float Mr = Mo + 1000 * re / (48.77 + prevDMC);
  float DMC = 244.72 - 43.43 * log(Mr - 20);
  return (rain > 1.5) ? DMC : prevDMC;
}

float calculateDC(float prevDC, float temp, float rain) {
  float re = (rain > 2.8) ? (rain - 2.8) : 0;
  float V = 0.36 * (temp + 2.8) + re;
  return prevDC + 0.5 * V;
}


float calculateISI(float ffmc, float wind) {
  float mo = 147.2 * (101.0 - ffmc) / (59.5 + ffmc);
  return 0.208 * exp(0.05039 * ffmc) * (1 + pow(wind, 0.5) / 25.0);
}

float calculateBUI(float DMC, float DC) {
  return (0.8 * DMC * DC) / (DMC + 0.4 * DC);
}

float calculateFWI(float ISI, float BUI) {
  float F = 0.1 * ISI * BUI;
  return F > 0 ? F / (0.1 + F) : 0;
}
