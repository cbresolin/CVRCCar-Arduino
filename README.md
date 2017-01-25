# CVRCCar-Arduino
Arduino code controlling a RC model car and detecting obstacles

![img_1097](https://cloud.githubusercontent.com/assets/19686240/22289701/4f6b66a2-e2fd-11e6-86bc-d92de522154c.JPG)

# Presentation
This project controls a RC car, sending PWM values to its 3 servo motors.
- throttle
- steering
- pan
Range of PWM values is determined by each servos.

PWM values are sent to the Arduino by an Android app.
(See https://github.com/cbresolin/CVRCCar-Android for more details)
Communication between the Android app and the Arduino is serial.
Arduino Uno board is powered using output of car's ESC controller.

![img_1095](https://cloud.githubusercontent.com/assets/19686240/22289767/8f5d2bc4-e2fd-11e6-9c2a-da67f85e0105.JPG)

# Features
## BT Communication
Hardware serial is used for communication using an HC-05 Bluetooth module.
This module is plugged into pin 0 (RX) & 1 (TX) of serial. BT communication enables
free movements of Android smartphone used to track an object (pan servo).

## PWM
The 3 PWM values are sent using a JSON structure to the Arduino.
If the serial buffer is not interpreted as valid JSON structure, it is ignored.

## Obstacle detection
2 HC-SR04 ultrasonic sensors are installed at the front of the RC car.
They have both a limit detection range of 30cm. Beyond that limit, no echoes
are returned.

If an echo is returned, this means that an obstacle is in front
of the car. In that case, this information is sent (via BT) to the Android app
to take care of.

Currently the Android app sets to neutral car's throttle when
an obstacle is detected.

# Libraries
- Servo (https://www.arduino.cc/en/Reference/Servo) for controlling servo motors
- NewPing (http://playground.arduino.cc/Code/NewPing) for controlling ultrasonic
sensors (e.g. HC-SR04)
- ArduinoJson (https://github.com/bblanchon/ArduinoJson) for JSON structure

# Reference
- https://github.com/bblanchon/ArduinoJson/wiki/Using%20the%20library%20with%20Arduino
- http://www.allaboutcircuits.com/projects/control-an-arduino-using-your-phone/
- http://eskimon.fr/2498-arduino-annexes-g-utiliser-module-bluetooth-hc-05
- https://docs.google.com/spreadsheets/d/16MIFE4ITEisEDUBh3H4A7WZga1Sm1Pm-igS3r0A58L8/pub?gid=0#
- https://oscarliang.com/arduino-timer-and-interrupt-tutorial/
- https://forum.arduino.cc/index.php?topic=358183.0
- http://forum.arduino.cc/index.php?topic=120013.0
