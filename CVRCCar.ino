/*
 Controlling a servo position using serial inputs
*/

#include <Servo.h>
#include <ArduinoJson.h>

#define SERIALBAUDRATE 9600

#define STEERINGPIN 9
#define STRAIGHT 1640 // 105°
#define MAXRIGHT 1910 // 141°
#define MAXLEFT 1370 // 71°
#define STEERINGRESPONSE 10
Servo steeringservo;

#define THROTTLEPIN 10
#define NEUTRAL 1490 // 86°
#define MAXTHROTTLE 1770 // 114°
#define MINFORWARD 1540 // 96°
#define MINREVERSE 1390 // 81°
#define MINTHROTTLE 1200 // 61°
#define THROTTLERESPONSE 10
Servo throttleservo;

StaticJsonBuffer<100> jsonBuffer;

void setup()
{
  // initialize serial communication:
  Serial.begin(SERIALBAUDRATE);
  
  // attaches steering servo
  steeringservo.attach(STEERINGPIN);
  set_steering(STRAIGHT);

  // attaches throttle servo
  throttleservo.attach(THROTTLEPIN);
  set_throttle(NEUTRAL);
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
      char json_buffer[60] = "";
      int pan;
      int tilt;
      int steering;
      int throttle;

      // If anything comes in Serial (USB)
      Serial.readBytesUntil("#", json_buffer, 60);
      
      // Test if parsing succeeds.
      // Example: {"pan":1543,"tilt":1774,"throttle":1490,"steering":1640}#
      JsonObject& root = jsonBuffer.parseObject(json_buffer);
      if (!root.success())
      {
        Serial.println("Parsing failed");
        return;
      }
      else
      {
        // Parse data
        pan = root["pan"];
        tilt = root["tilt"];
        throttle = root["throttle"];
        steering = root["steering"];
      }

      // Act on steering
      set_steering(steering);

      // Act on throttle
      set_throttle(throttle);
    }
}

