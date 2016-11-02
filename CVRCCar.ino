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
int steering = STRAIGHT;

#define THROTTLEPIN 10
#define NEUTRAL 1490 // 86°
#define MAXTHROTTLE 1770 // 114°
#define MINFORWARD 1540 // 96°
#define MINREVERSE 1390 // 81°
#define MINTHROTTLE 1200 // 61°
#define THROTTLERESPONSE 10
Servo throttleservo;
int throttle = NEUTRAL;

StaticJsonBuffer<200> jsonBuffer;

void setup()
{
  // initialize serial communication:
  Serial.begin(SERIALBAUDRATE);
  
  // attaches steering servo
  steeringservo.attach(STEERINGPIN);
  writeservo(steeringservo, steering);

  // attaches throttle servo
  throttleservo.attach(THROTTLEPIN);
  writeservo(throttleservo, throttle);
}

void steer_right()
{ 
  if (steering < MAXRIGHT)
  {
    steering += STEERINGRESPONSE;
    writeservo(steeringservo, steering);
  }
}

void steer_left()
{
  if (steering > MAXLEFT)
  {
    steering -= STEERINGRESPONSE;
    writeservo(steeringservo, steering);
  }
  
}

void throttle_up()
{
  if (throttle < MAXTHROTTLE)
  {
    throttle += THROTTLERESPONSE;
    writeservo(throttleservo, throttle);
  }
}

void throttle_down()
{
  if (throttle > MINTHROTTLE)
  {
    throttle -= THROTTLERESPONSE;
    writeservo(throttleservo, throttle);
  }
}

void writeservo(Servo servo, int time)
{
  servo.writeMicroseconds(time);
  delay(15);
}

int readservo(Servo servo)
{
  return servo.read();
}

// the loop routine runs over and over again forever:
void loop()
{
    if (Serial.available())
    { 
      // If anything comes in Serial (USB)
      char input = Serial.read();
  
      switch (input)
      {
        case '4':
          steer_left();
          break;
        case '6':
          steer_right();
          break;
          case '-':
          throttle_down();
          break;
        case '+':
          throttle_up();
          break;
        default: 
          // Nothing
        break;
      }
      
      // Debug
      Serial.print("Input = ");
      Serial.print(input); 
      Serial.print(", Steering = ");
      Serial.print(steering);
      Serial.print(", Throttle = ");
      Serial.println(throttle);
    }
}

