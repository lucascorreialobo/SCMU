// The API key can be obtained from Firebase console > Project Overview > Project settings.
#define API_KEY "AIzaSyDnhI7ErwlSY-T3j6z1EUt1tpNE7QSsWEQ"

// User Email and password that already registerd or added in your project.
#define USER_EMAIL "ll@avernos.com"
#define USER_PASSWORD "Admin00"
#define DATABASE_URL "https://averno-scmu-default-rtdb.europe-west1.firebasedatabase.app/"

#define VALUES_PER_SENSOR 9

#define FIREBASE_PROJECT_ID "averno-scmu"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-a6wrc@averno-scmu.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\n\
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDBdUtNTm5gZ/Nc\n\
hebv5ah3aGsUz87b2yYRVnqDvvH1FSmA7h0ujZQ7p79lD2Es+hM8le4dxSVdyZcz\n\
i42x6lFHtnMsxDXEib7I2tmK0HsWM1pH9jKUUl3e74iPSZEAF+GWCfWSoWHarYmH\n\
KZAsOr42rw6Md9SspiHNixgkdoMRjhF5sRsOBjD7b5nrLELaPg1lrlE7bDIDMLGH\n\
yIUTCqBI8SGcU69vOOzNMnqkMZN7yEvAu+0nmr3FYTJu9M2nUmCIwbSloI6/F2nk\n\
ySW+cm8TzulHHnJUarg849tzzTVeodTZ8UfPlCyqMIb76srM4pyzqgZDr8IUGVpQ\n\
7KrfvRBxAgMBAAECggEAB/u/qkaIzoirpc9OyuqWy6qu9JJoznDaPs/rSWGJWZxQ\n\
dKYe5StpiRawksqSL4l4XsEvLOJUWuz1RObG56csSIHSee/06hHnIiHJ5W0aDz36\n\
RUr3JmDJx+fpokQebDsQD6GElVL8l0ofCUlhTjn0XIxX1NsjFybZ8UICsoqDeSdp\n\
zDhufD5VE0vYzqfjNoVNI8Lh4v6HM01mHuEh1/aEkh6hD3cGhJoUwlprRlAhCqzG\n\
Wy7LwfNgK7cDXkMw6AarH8KTaQfS1iddrDsObR+ATr1v3YmskF+vuHVXwRNmLXCm\n\
PPPHrHoxlxQCSra7RL9pSlyoISLJ5wJ1WJmkFFfE+wKBgQDwUtq2lOObraD+sXsP\n\
qhiZxArpzKWLnTH/YTfjUk/zcjLFav1uhHf2Pzk0G+dhZVnY38ISxKxo/XsnZZch\n\
HGNVYjSJKz1LKMSB1kHfeRwwrlpdoCKWMW9D7F8FjttAMcFEKZ37I7l83J8UZiRV\n\
Qlf2uQzkaKtf7Hxv8FupZODoywKBgQDOE9ZHQUHib/AzJlD+VK68jwk9s1zybJBt\n\
f3ipmbI/Wt2ssIxBzd+9Y1BZEC3cEBtWP8K4kg69XkqPoAHLGKz1bv4UXybWi18u\n\
VJmroa6Em+hA+GKZuu4xhTCF9tIhBEkk9ZzWCqpz2/51HqWpgoarTRzHMZa4N5Nq\n\
iY3dduYQMwKBgQDYdXhD3cVIklIhNZCnIkj4Lqi5vzcYkmTXJiSlnp0epCxwvpY4\n\
Nh6q8HVApyV4B0au3ZO7PeQgyLdlW86M5zndTRAOwKB7kuO0IlzAwSAQBOqly7d7\n\
Wllib4OCyRJYNrhiAa4m5x2KBllOkVWGabQ7KKAhFtnNE39df7u/znn5kwKBgFil\n\
7+d3CwB9ZraaJP1DMGesoC0Km1EtonN4Ak/y7dqTHAjNaYhlJ3ow3r9xVkj0JM5V\n\
X05GhlCv3yMpkZvsYXqy5FmfFPX1vlwPyIssYgZpit9n+gJnh0+GG/mE05+gWFat\n\
0WnB7NEqMavwIxNoMsY9xcNqcYuNXLzXr081doJBAoGBAO6vU0mAI/BJApPEqdKm\n\
pk3XI2bD98AVGqaS8ilWKmZhHXq+KO+YYW0anfk4PE4QY+lqfXlV6Nr5pJJgFfLQ\n\
iRAYbiu9IjXroSHaM8NpmH4AZcA6ZNRDFSpVgBIf4tGTAG0vxztkTcqgJ/uzR25/\n\
Bz3exgbxrgi1GzqIVw0y2TGg\n\
-----END PRIVATE KEY-----\n";

void authHandler();

void getMsg(Messages::Message &msg);

void timeStatusCB(uint32_t &ts);

void asyncCB(AsyncResult &aResult);

void printResult(AsyncResult &aResult);

void printError(int code, const String &msg);

DefaultNetwork network;

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

ServiceAuth sa_auth(timeStatusCB, FIREBASE_CLIENT_EMAIL, FIREBASE_PROJECT_ID, PRIVATE_KEY, 3000 /* expire period in seconds (<= 3600) */);


FirebaseApp app;

#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

Messaging messaging;

AsyncResult aResult_no_callback;

bool taskComplete = false;
bool infoSent = false; 
int tasksCompleted = 0;

// is same as taskComplete?
bool taskCompleted = false;

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

void FCM_setup(){
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  Serial.println("Initializing app...");

  ssl_client.setInsecure();


  initializeApp(aClient, app, getAuth(sa_auth), aResult_no_callback);

  authHandler();

  // Binding the FirebaseApp for authentication handler.
  // To unbind, use messaging.resetApp();
  app.getApp<Messaging>(messaging);

  // In case setting the external async result to the sync task (optional)
  // To unset, use unsetAsyncResult().
  aClient.setAsyncResult(aResult_no_callback);
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
        if(++tasksCompleted >= sensorDataCounter * VALUES_PER_SENSOR){
          infoSent = true;
        }
        Serial.printf("%d tasks completed. %d to go", tasksCompleted, sensorDataCounter * VALUES_PER_SENSOR - tasksCompleted);
    }
}

void printError(int code, const String &msg)
{
    Firebase.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}

void FCM_send_message(String title, String body){
  setUpWifi();

  FCM_setup();

  while(!taskCompleted){
    authHandler();
    messaging.loop();

    if (app.ready() && !taskCompleted)
    {
        taskCompleted = true;

        Serial.println("Sending message...");

        Messages::Message msg;
        getMsg(msg, title, body);

        // You can set the content of msg object directly with msg.setContent("your content")

        String payload = messaging.send(aClient, Messages::Parent(FIREBASE_PROJECT_ID), msg);

        if (aClient.lastError().code() == 0)
            Serial.println(payload);
        else
            printError(aClient.lastError().code(), aClient.lastError().message());
    }
  }

  disconnect_WiFi();
}


void firebase_code(){
  setUpWifi();
  FirebaseSetUp();

  if(!app.ready()){
    //Serial.println("App is not ready yet, please wait a bit.");
  }

  while(!infoSent){
    app.loop();

    Database.loop();

    if (app.ready() && !taskComplete)
    {
      sendDataOnArray();
      // app.loop();

      // Database.loop();
      // testFirebaseGetSend();
      // break;
    }
  }


  
}

void sendDataOnArray() {

  taskComplete = true;

  // setUpWifi();
  setup_Time();

  for(int i = 0; i < sensorDataCounter; i++){
    SensorData temp = sensorDataArray[i];
    String id = macToString(temp.macAddress);
    String timeString = getFormattedTimeString(printLocalTime()); 


    String devicePath = "/forests/" + forestID + "/" + id + "/";

    String timedDevicePath = devicePath + "data/" + timeString + "/";

    Serial.println(timedDevicePath);

    Database.set<String>(aClient, devicePath + "coordinates/latitude", temp.coordinates.latitude, asyncCB, "setLatitude");
    Database.set<String>(aClient, devicePath + "coordinates/lonngitude", temp.coordinates.longitude, asyncCB, "setLongitude");

    float tempC = temp.temperatureC;

    Database.set<float>(aClient, timedDevicePath + "temperature (°C)",  22.0, asyncCB, "setTemperatureC");
    Database.set<float>(aClient, timedDevicePath + "temperature (°F)",  22.0, asyncCB, "setTemperatureF");
    Database.set<float>(aClient, timedDevicePath + "humidity",          22.0, asyncCB, "setHumidity");
    Database.set<float>(aClient, timedDevicePath + "gas",               22.0, asyncCB, "setGas");
    Database.set<float>(aClient, timedDevicePath + "wind speed",        22.0, asyncCB, "setWindSpeed");
    Database.set<bool> (aClient, timedDevicePath + "smoke danger",     false, asyncCB, "setIsSmokeDanger");
    Database.set<float>(aClient, timedDevicePath + "local FWI",         22.0, asyncCB, "setFWI");
  }

  // Database.set<int>(aClient, "/test/int", 22, asyncCB, "setIntTask");
  // disconnect_WiFi();
}

void authHandler()
{
    // Blocking authentication handler with timeout
    unsigned long ms = millis();
    while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000)
    {
        // The JWT token processor required for ServiceAuth and CustomAuth authentications.
        // JWT is a static object of JWTClass and it's not thread safe.
        // In multi-threaded operations (multi-FirebaseApp), you have to define JWTClass for each FirebaseApp,
        // and set it to the FirebaseApp via FirebaseApp::setJWTProcessor(<JWTClass>), before calling initializeApp.
        JWT.loop(app.getAuth());
        printResult(aResult_no_callback);
    }
}

void getMsg(Messages::Message &msg, String title, String body)
{
    msg.topic("test");
    // msg.token("DEVICE_TOKEN"); // Registration token to send a message to
    msg.condition("'sensor_alerts' in topics");

    // Basic notification
    Messages::Notification notification;
    notification.body(title).title(body);

    // Library does not provide JSON parser library, the following JSON writer class will be used with
    // object_t for simple demonstration.

    object_t data, obj1, obj2, obj3, obj4;
    JsonWriter writer;

    writer.create(obj1, "name", string_t("wrench"));
    writer.create(obj2, "mass", string_t("1.3kg"));
    writer.create(obj3, "count", string_t("3"));
    writer.join(data, 3 /* no. of object_t (s) to join */, obj1, obj2, obj3);

    // object_t data2("{\"name\":\"wrench\",\"mass\":\"1.3kg\",\"count\":\"3\"}");

    msg.data(data);

    data.clear();

    Messages::AndroidConfig androidConfig;

    // Priority of a message to send to Android devices.
    // https://firebase.google.com/docs/reference/fcm/rest/v1/projects.messages#androidmessagepriority
    androidConfig.priority(Messages::AndroidMessagePriority::_HIGH);

    Messages::AndroidNotification androidNotification;

    // Set the relative priority for this notification.
    // Priority is an indication of how much of the user's attention should be consumed by this notification.
    // Low-priority notifications may be hidden from the user in certain situations,
    // while the user might be interrupted for a higher-priority notification.
    // https://firebase.google.com/docs/reference/fcm/rest/v1/projects.messages#NotificationPriority
    androidNotification.notification_priority(Messages::NotificationPriority::PRIORITY_HIGH);

    androidConfig.notification(androidNotification);

    msg.android(androidConfig);

    msg.notification(notification);
}

void timeStatusCB(uint32_t &ts)
{
#if defined(ESP8266) || defined(ESP32) || defined(CORE_ARDUINO_PICO)
    if (time(nullptr) < FIREBASE_DEFAULT_TS)
    {

        configTime(3 * 3600, 0, "pool.ntp.org");
        while (time(nullptr) < FIREBASE_DEFAULT_TS)
        {
            delay(100);
        }
    }
    ts = time(nullptr);
#elif __has_include(<WiFiNINA.h>) || __has_include(<WiFi101.h>)
    ts = WiFi.getTime();
#endif
}

String getFormattedTimeString(struct tm timeinfo) {
    char buffer[30]; // Buffer to hold the formatted string
    strftime(buffer, sizeof(buffer), "%Y %m %d %H:%M:%S", &timeinfo);
    return String(buffer);
}

