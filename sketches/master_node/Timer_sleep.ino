//Time-to-Sleep
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

void start_sleep_for(int time){
  preferences.begin("Averno", false);
  preferences.putBool("Skip Startup", true); //by default set skip startup to false, if going to forced sleep set to true
  preferences.end();

  esp_sleep_enable_timer_wakeup(time * uS_TO_S_FACTOR /*/ 4*/); // time set with variable above
  esp_deep_sleep_start();
}


