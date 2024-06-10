#define SMOKE_THRESHOLD 300

#define MODERATE_DANGER  8.2 <= FWI < 17.2
#define HIGH_DANGER  17.2 <= FWI < 38.3
#define VERY_HIGH_DANGER  24.6 <= FWI < 38.3
#define MAXIMUM_DANGER  38.3 <= FWI < 50.1
#define EXTREME_DANGER  50.1 <= FWI < 64
#define EXCEPTIONAL_DANGER  64 <= FWI

// Initialize Fire Weather Index components (use starting values or load from storage)
const float DEFAULT_FFMC = 85.0; // Example starting value
const float DEFAULT_DMC = 6.0;  // Example starting value
const float DEFAULT_DC = 15.0;  // Example starting value
const float DEFAULT_ISI = 5.0;  // Example starting value
const float DEFAULT_BUI = 20.0; // Example starting value

struct SensorData sensor_data;

float randomFloat(float min, float max) {
    return random(min * 100, max * 100) / 100.0;
}

struct SensorData get_sensor_data(){
  DHTData dht = get_DHT_values();
  int gasValue = get_MQ2_values();

  getMacAddress(sensor_data.macAddress);
  sensor_data.coordinates = getPreferencesCoordinates();
  sensor_data.temperatureC = isnan(dht.temperatureC) ? randomFloat(-10.0, 40.0) : dht.temperatureC;
  sensor_data.temperatureF = isnan(dht.temperatureF) ? sensor_data.temperatureC * (9.0 / 5.0) + 32.0 : dht.temperatureF;
  sensor_data.humidity = isnan(dht.humidity) ? randomFloat(0.0, 100.0) : dht.humidity;
  sensor_data.gas = isnan(gasValue) ? randomFloat(0.0, 10.0) : gasValue;
  sensor_data.windSpeed = randomFloat(0.0, 100.0);
  sensor_data.rain = randomFloat(0.0, 50.0);
  sensor_data.local_FWI = fwi_calc();
  sensor_data.isSmokeDanger = isDangerSmoke();

  return sensor_data;
}

bool isDangerSmoke(){
   // Include logic to determine if smoke levels are indicative of a fire
  bool possible_fire = sensor_data.gas > SMOKE_THRESHOLD; // Define a suitable threshold based on calibration

  if (possible_fire) {
    Serial.println("Warning: High smoke levels detected!");
    // sendNotification("Alert", "Sensor value exceeded threshold!");
    FCM_send_message("Alert!", "Warning: High smoke levels detected!");
  }

  return possible_fire;
}

float fwi_calc(){
  // Read sensors
  float temperature = sensor_data.temperatureC;
  float humidity = sensor_data.humidity;
  float windSpeed = sensor_data.windSpeed;
  float rain = sensor_data.rain;

  preferences.begin("Averno", false);
  // Calculate FWI components (simplified)
  // This requires proper formulas based on the Canadian Forest Fire Weather Index System
  float FFMC = calculateFFMC(preferences.getFloat("FFMC", DEFAULT_FFMC), temperature, humidity, windSpeed, rain);
  float DMC = calculateDMC(preferences.getFloat("DMC", DEFAULT_DMC), temperature, humidity, rain);
  float DC = calculateDC(preferences.getFloat("DC", DEFAULT_DC), temperature, rain);
  float ISI = calculateISI(FFMC, windSpeed);
  float BUI = calculateBUI(DMC, DC);

  // Calculate the FWI based on the components
  float FWI = calculateFWI(ISI, BUI);
  preferences.end();

  // Output the result
  Serial.print("FWI: ");
  Serial.println(FWI);

  if( MODERATE_DANGER || HIGH_DANGER || VERY_HIGH_DANGER){
    Serial.println("Warning: High intensity fires, including crown fires!");
    FCM_send_message("Alert!", "Warning: High intensity fires, including crown fires detected!");
  }
  if( MAXIMUM_DANGER ){
    Serial.println("Warning: 	Extremely intense fires with active crown fires, spotting and high dificulty for fire management!");
    FCM_send_message("Alert!", "Warning: 	Extremely intense fires with active crown fires, spotting and high dificulty for fire management!");
  }
  if( EXTREME_DANGER || EXCEPTIONAL_DANGER){
    Serial.println("Warning: 	Exceptional intense fires with extreme dificulty for fire management!");
    FCM_send_message("Alert!", "Warning: 	Exceptional intense fires with extreme dificulty for fire management!");
  }

  return FWI;
}

float calculateFFMC(float prevFFMC, float temp, float hum, float wind, float rain) {
    float prev_mt = 147.2 * ((101.0 - prevFFMC)/(59.5 + prevFFMC));
  
    float mrt = prev_mt;
    if(rain > 0.5){
        float pf = rain - 0.5;
        mrt = prev_mt + 42.5 * pf * exp((-100.0 + prev_mt) / 251.0) * (1.0 - exp(-6.93 / pf));
        if(prev_mt > 150.0){
            mrt = mrt + 0.0015 * pow(prev_mt - 150.0, 2) * pow(pf, 0.5);
        }
    }

    mrt = mrt > 250.0 ? 250.0 : mrt;

    float ed = 0.942 * pow(hum, 0.679) + 11.0 * exp((hum - 100.0) / 10.0) + 0.18 * (21.1 - temp) * (1.0 - exp(-0.115 * hum));

    float m = prev_mt;
    if(ed < prev_mt){
        float ko = 0.424 * (1.0 - pow(hum / 100.0, 1.7)) + 0.0694 * pow(wind, 0.5) * (1.0 - pow(hum / 100.0, 8.0));
        float kd = ko * 0.581 * exp(0.0365 * temp);
        m = ed + (prev_mt - ed) * pow(10.0, -kd);
    } else {
        float ew = 0.618 * pow(hum, 0.753) + 10.0 * exp((hum - 100.0) / 10.0) + 0.18 * (21.1 - temp) * (1.0 - exp(-0.115 * hum));
        if(ew > prev_mt){
            float k1 = 0.424 * (1.0 - pow((100.0 - hum) / 100.0, 1.7)) + 0.0694 * pow(wind, 0.5) * (1.0 - pow(hum / 100.0, 8.0));
            float kw = k1 * 0.581 * exp(0.0365 * temp);
            if(ew <= prev_mt && prev_mt <= ed){
                m = prev_mt;
            } else {
                m = ew - (ew - prev_mt) * pow(10.0, -kw);
            }
        }
    }

    float ffmc = 59.5 * ((250.0 - m) / (147.2 + m));
    return ffmc;
}

float calc_b(float prevDMC){
  if( prevDMC <= 33 ){
    return 100/(0.5 + 0.3 * prevDMC);
  }

  if( prevDMC > 65){
    return 6.2 * log(prevDMC) - 17.2;
  }

  return 14 - 1.3 * log(prevDMC);
}

float calculateDMC(float prevDMC, float temp, float hum, float rain) {

  float lf = 13.9; // lf should have a different value for each month but since we do not have access to the time in the worker node we'll use the value for june

  temp = temp >= -1.1 ? temp : -1.1;
  float k = 1.894 * (temp  + 1.1) * (100 - hum) * lf * pow(10, -6);

  float dmc = 0;
  if(rain <= 1.5){
    dmc = prevDMC + 100 * k;
  }
  else
  { 
    float pe = 0.92 * rain - 1.27;
    float prevMrt = 20 + exp(5.6348 - prevDMC/43.43);
    float b = calc_b(prevDMC);
    float mrt = prevMrt + (100 * pe)/(48.77 + b * pe);

    float dmcrt_val = 244.72 - 43.43 * log(mrt - 20); 
    float dmcrt = dmcrt_val >= 0 ? dmcrt_val : 0;

    dmc = dmcrt + 100 * k;
  }

  preferences.putFloat("DMC", dmc);
  return dmc;

  // float re = (rain > 1.5) ? (rain - 1.5) : 0;
  // float Mo = 20 + exp(5.6348 - prevDMC / 43.43);
  // float Mr = Mo + 1000 * re / (48.77 + prevDMC);
  // float DMC = 244.72 - 43.43 * log(Mr - 20);
  // return (rain > 1.5) ? DMC : prevDMC;
}

float calculateDC(float prevDC, float temp, float rain) {

  float lf = 5.8; // lf should have a different value for each month but since we do not have access to the time in the worker node we'll use the value for june

  temp = temp >= -2.8 ? temp : -2.8;
  float v_val = 0.36 * (temp + 2.8) + lf; 
  float v = v_val > 0 ? v_val : 0;

  float dc = 0;
  if(rain <= 2.8){
    dc = prevDC + 0.5 * v;
  }
  else{
    float pd = 0.83 * rain - 1.27;

    float prevQrt = 800 * exp(-prevDC/400);
    float qrt = prevQrt + 3.937 * pd;

    float dcrt_val = 400 * log(800 / qrt);
    float dcrt = dcrt_val > 0 ? dcrt_val : 0;

    dc = dcrt + 0.5 * v;
  }
  
  preferences.putFloat("DC", dc);
  return dc;
  // float re = (rain > 2.8) ? (rain - 2.8) : 0;
  // float V = 0.36 * (temp + 2.8) + re;
  // return prevDC + 0.5 * V;
}


float calculateISI(float FFMC, float wind) {

  float fu = exp(0.05039 * wind);
  float m = 147.2 * ((101 - FFMC)/(59.5 + FFMC));
  float ff = (91.9 * exp(-0.1386 * m)) * (1 + (pow(m, 5.31)/(4.93 * pow(10, 7))));

  return 0.208 * fu * ff;
}

float calculateBUI(float DMC, float DC) {
  
  if(DMC <= 0.4 * DC){
    return 0.8 * ((DMC * DC)/(DMC + 0.4 * DC));
  }else{
    return DMC - (1 - ((0.8 * DC)/(DMC + 0.4 * DC))) * (0.92 + pow(0.0114 * DMC, 1.7));
  }
  // return (0.8 * DMC * DC) / (DMC + 0.4 * DC);
}

float calculateFWI(float ISI, float BUI) {
  Serial.print("ISI: ");
  Serial.println(ISI);
  Serial.print("BUI: ");
  Serial.println(BUI);
  float fd = 0;
  if(BUI <= 80){
    fd = 0.626 * pow(BUI, 0.809) + 2;
  }
  else{
    fd = 1000 / (25 + 108.64 * exp(-0.023 * BUI));
  }
  float b = 0.1 * ISI * fd;
  if(b > 1){
    return exp(2.72 * pow(0.434 * log(b), 0.647));
  }
  else{
    return b;
  }
}
