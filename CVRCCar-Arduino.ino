#include <Servo.h>
#include <NewPing.h>
#include <ArduinoJson.h>

#define SERIALBAUDRATE 9600
#define TRIGGERPIN 4
#define ECHOPIN 7
#define MAXDISTANCE 150 // 1.5 meters
#define STOPDISTANCE 15 // 15 centimeters

NewPing sonar(TRIGGERPIN, ECHOPIN, MAXDISTANCE);

unsigned int pingSpeed = 500; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;       // Holds the next ping time.

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

  pingTimer = millis(); // Start now.
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

void echoCheck()
{ // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  if (sonar.check_timer())
  { // This is how you check to see if the ping was received.
    int unsigned dist = NewPingConvert(sonar.ping_result, US_ROUNDTRIP_CM);
    Serial.println(dist); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
  }
}

// the loop routine runs over and over again forever:
void loop()
{
  // Notice how there's no delays in this sketch to allow you to do other processing in-line while doing distance pings.
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
  
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

