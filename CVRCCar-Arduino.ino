/*
 Controlling a servo position using serial inputs
*/

#include <Servo.h>
#include <ArduinoJson.h>

#define SERIALBAUDRATE 9600

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
  panservo.attach(PANPIN, 565, 2250);
  set_pan(PANSTRAIGHT);
  
  // attaches steering servo
  steeringservo.attach(STEERINGPIN, 1340, 1920);
  set_steering(STRAIGHT);

  // attaches throttle servo
  throttleservo.attach(THROTTLEPIN, 1370, 1560);
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

