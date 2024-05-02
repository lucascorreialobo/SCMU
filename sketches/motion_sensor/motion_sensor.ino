
#define timeSeconds 5

const int led = 26;
const int motionSensor = 27;

// Timer: Auxiliar variables
long now = millis();
long lastTrigger = 0;
boolean startTimer = false;

boolean motionDetectedFlag = false;

//checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement(){
  motionDetectedFlag = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(motionSensor, INPUT_PULLUP);

  //Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // set LED LOW
  pinMode(led, OUTPUT);  
}

void loop() {
  // Current time
  now = millis();
  //Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))){
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
  }

  if(motionDetectedFlag){
    turnLEDOn();
  }
}

void turnLEDOn(){
    Serial.println("MOTION DETECTED!!!");
    digitalWrite(led, HIGH);
    startTimer = true;
    lastTrigger = millis();
    motionDetectedFlag = false;
}





















