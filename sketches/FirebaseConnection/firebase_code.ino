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

  Database.get(aClient, "/forests/{}/{time}/", asyncCB, false, "getTask1");
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
  if (app.ready() && !taskComplete)
  {
    testFirebaseGetSend();
  }
}
