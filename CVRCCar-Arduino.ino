/*
 Controlling a servo position using serial inputs
*/

#include <Servo.h>
#include <NewPing.h>
#include <ArduinoJson.h>

#define SERIALBAUDRATE 9600
#define TRIGGER_PIN  3  
#define ECHO_PIN     2  
#define MAX_DISTANCE 200
#define STOP_DISTANCE 15

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define PANPIN 9
#define PANSTRAIGHT 1385 // 82°
Servo panservo;

#define STEERINGPIN 10
#define STRAIGHT 1640 // 105°
Servo steeringservo;

#define THROTTLEPIN 11
#define NEUTRAL 1490 // 86°
Servo throttleservo;

void setup()
{
  // initialize BT serial communication:
  Serial.begin(SERIALBAUDRATE);
  
  // attaches pan servo
  panservo.attach(PANPIN);
  set_pan(PANSTRAIGHT);
  
  // attaches steering servo
  steeringservo.attach(STEERINGPIN);
  set_steering(STRAIGHT);

  // attaches throttle servo
  throttleservo.attach(THROTTLEPIN);
  set_throttle(NEUTRAL);
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

// the loop routine runs over and over again forever:
void loop()
{                     
  
  // Measure distance to nearest obstacle
  int dist_to_obstacle = sonar.ping_cm();
  delay(50);
  
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
    else if (dist_to_obstacle >= STOP_DISTANCE)
    {
      // Act on pan
      set_pan(root["pan"]);
        
      // Act on steering
      set_steering(root["steering"]);

      // Act on throttle
      set_throttle(root["throttle"]);
    }
    else
    {
      // Act on throttle
      set_throttle(NEUTRAL);
    }
  }
}

