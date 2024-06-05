
// The API key can be obtained from Firebase console > Project Overview > Project settings.
#define API_KEY "AIzaSyDnhI7ErwlSY-T3j6z1EUt1tpNE7QSsWEQ"

// User Email and password that already registerd or added in your project.
#define USER_EMAIL "ll@avernos.com"
#define USER_PASSWORD "Admin00"
#define DATABASE_URL "https://averno-scmu-default-rtdb.europe-west1.firebasedatabase.app/"

void asyncCB(AsyncResult &aResult);

void printResult(AsyncResult &aResult);

DefaultNetwork network;

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);


FirebaseApp app;

#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

bool taskComplete = false;

void FirebaseSetUp(){
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  Serial.println("Initializing app...");

  ssl_client.setInsecure();

  initializeApp(aClient, app, getAuth(user_auth), asyncCB, "authTask");

  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);
}

void startAsyncFirebase(){
  app.loop();

  Database.loop();
}


void testFirebaseGetSend(){
  
  
  taskComplete = true;

  Database.get(aClient, "/forests", asyncCB, false, "getTask1");
  Database.set<int>(aClient, "/test/int", 22, asyncCB, "setIntTask");

  int time;


  //Database.get(aClient, "/test/string", asyncCB, false, "getTask2");

  // Filtering data
  // For REST API, indexing the data at /test/filter/json is required when filtering the data, please see examples/Database/Extras/Sync/IndexingData/IndexingData.ino.
  DatabaseOptions options;
  options.filter.orderBy("Data").startAt(105).endAt(120).limitToLast(8);

  Database.get(aClient, "/test/filter/json", options, asyncCB, "getTask3");
  

}

void asyncCB(AsyncResult &aResult)
{
    // WARNING!
    // Do not put your codes inside the callback and printResult.

    printResult(aResult);
}


void printResult(AsyncResult &aResult)
{
    if (aResult.isEvent())
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }

    if (aResult.available())
    {
        Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    }
}


void firebase_code(){

  if(!app.ready()){
    //Serial.println("App is not ready yet, please wait a bit.");
  }

  if (app.ready() && !taskComplete)
  {
    sendDataOnArray();

    testFirebaseGetSend();
  }

  
}

void sendDataOnArray() {

  for(int i = 0; i < sensorDataCounter; i++){
    SensorData temp = sensorDataArray[i];
    String id = macToString(temp.macAddress);
    String timeString = getFormattedTimeString(temp.timeInfo);


    String devicePath = "/forests/" + forestID + "/" + id + "/" + timeString + "/";

    Database.set<float>(aClient, )
  }

  // Database.set<int>(aClient, "/test/int", 22, asyncCB, "setIntTask");

}

String getFormattedTimeString(const struct tm *timeinfo) {
    char buffer[30]; // Buffer to hold the formatted string
    strftime(buffer, sizeof(buffer), "%d %B %Y %H:%M:%S", timeinfo);
    return String(buffer);
}

// struct SensorData {
//   uint8_t macAddress[6]; // MAC address field
//   float temperatureC; // temperature reading in Celsius
//   float temperatureF; // temperature reading in Fahrenheit
//   float humidity;    // humidity reading
//   float gas;
//   float windSpeed;
//   float rain;
//   bool isSmokeDanger; 
//   float local_FWI;
//   struct tm timeinfo;
// };
