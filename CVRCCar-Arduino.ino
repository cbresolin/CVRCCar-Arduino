/*
 Controlling a servo position using serial inputs
*/

#include <Servo.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define SERIALBAUDRATE 9600
#define PAYLOADSIZE 3

#define RXPIN 2
#define TXPIN 4
SoftwareSerial btserial(RXPIN, TXPIN);

#define PANPIN 11
#define PANSTRAIGHT 1385 // 82°
#define PANMAXRIGHT 2250 // 167°
#define PANMAXLEFT 565 // 2°
Servo panservo;

#define STEERINGPIN 10
#define STRAIGHT 1640 // 105°
#define MAXRIGHT 1910 // 141°
#define MAXLEFT 1370 // 71°
Servo steeringservo;

#define THROTTLEPIN 9
#define NEUTRAL 1490 // 86°
#define MAXTHROTTLE 1770 // 114°
#define MINFORWARD 1540 // 96°
#define MINREVERSE 1390 // 81°
#define MINTHROTTLE 1200 // 61°
Servo throttleservo;

void setup()
{
  // initialize BT serial communication:
  btserial.begin(SERIALBAUDRATE);
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
  if (btserial.available())
  {
    char serialbuffer[45] = "";
    DynamicJsonBuffer jsonBuffer;  

    // If anything comes in Serial (USB)
    btserial.readBytesUntil(';', serialbuffer, sizeof(serialbuffer));
            
    Serial.print("serialbuffer = ");
    Serial.println(serialbuffer);
    Serial.print("btserial.read() = ");
    Serial.println(btserial.read());
    
    // Test if parsing succeeds.
    // Example: {"pan":1385,"steering":1640,"throttle":1490};
    JsonObject& root = jsonBuffer.parseObject(serialbuffer);
    if (!root.success())
    {
      // Nothing is sent
      Serial.print(serialbuffer);
      Serial.println(" not JSON structure!");
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

