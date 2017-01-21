#include <Servo.h>
#include <NewPing.h>
#include <ArduinoJson.h>

#define SERIALBAUDRATE 9600
#define SONAR_NUM     2   // Number or sensors.
#define MAX_DISTANCE  30  // Max distance in cm.
#define PING_INTERVAL 150 // Milliseconds between pings.

unsigned long pingTimer[SONAR_NUM]; // When each pings.
unsigned int cm[SONAR_NUM];         // Store ping distances.
uint8_t currentSensor = 0;          // Which sensor is active.
String id[SONAR_NUM] = {"L", "R"};  // Store sonar names (Left, Right)
boolean is_obstacle = false;

NewPing sonar[SONAR_NUM] = { // Sensor object array.
  NewPing(2, 3, MAX_DISTANCE),
  NewPing(4, 5, MAX_DISTANCE)
};

#define PANPIN 9
#define PANSTRAIGHT 1385 // 82°
Servo panservo;

#define STEERINGPIN 10
#define STRAIGHT 1640 // 105°
Servo steeringservo;

#define THROTTLEPIN 12
#define NEUTRAL 1490 // 86°
Servo throttleservo;

void setup()
{
  // initialize BT serial communication:
  Serial.begin(SERIALBAUDRATE);
  
  // attaches pan servo
  panservo.attach(PANPIN, 565, 2250);
  set_pan(PANSTRAIGHT);
  
  // attaches steering servo
  steeringservo.attach(STEERINGPIN, 1340, 1920);
  set_steering(STRAIGHT);

  // attaches throttle servo
  throttleservo.attach(THROTTLEPIN, 1370, 1560);
  set_throttle(NEUTRAL);

  pingTimer[0] = millis() + 75; // First ping start in ms.
  for (uint8_t i = 1; i < SONAR_NUM; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void set_pan(int pwm)
{ 
  writeservo(panservo, pwm);
}

void set_steering(int pwm)
{ 
  writeservo(steeringservo, pwm);
}

void set_throttle(int pwm)
{
  writeservo(throttleservo, pwm);
}

void writeservo(Servo servo, int time)
{
  servo.writeMicroseconds(time);
  delay(15);
}

void sonarloop() {
  is_obstacle = false;
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
      if (i == 0 && currentSensor == SONAR_NUM - 1)
        oneSensorCycle(); // Do something with results.
      sonar[currentSensor].timer_stop();
      currentSensor = i;
      // cm[currentSensor] = sonar[currentSensor].ping_median(3) / US_ROUNDTRIP_CM;
      cm[currentSensor] = 0;
      sonar[currentSensor].ping_timer(echoCheck);
    }
  }
}
 
void echoCheck() { // If ping echo, set distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}
 
void oneSensorCycle() { 
  // Do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    if (cm[i] != 0)
       is_obstacle = true;

//    Serial.print(id[i]);
//    Serial.print("=");
//    Serial.print(cm[i]);
//    Serial.print(" ");
  }
  Serial.print(is_obstacle);
}

void drive()
{ 
  if (Serial.available())
  {
    char serialbuffer[45] = "";
    DynamicJsonBuffer jsonBuffer;  

    // If anything comes in Serial (USB)
    Serial.readBytesUntil(';', serialbuffer, sizeof(serialbuffer));
    
    // Test if parsing succeeds.
    // Example: {"pan":1385,"steering":1640,"throttle":1490};
    JsonObject& root = jsonBuffer.parseObject(serialbuffer);
    if (!root.success())
    {
      // Nothing is sent
      return;
    }
    else
    {
      // Act on pan
      set_pan(root["pan"]);
        
      // Act on steering
      set_steering(root["steering"]);
      
      // Act on throttle
      set_throttle(root["throttle"]);  
    }
  }
}

// the loop routine runs over and over again forever:
void loop()
{
  sonarloop();
  drive();
}
