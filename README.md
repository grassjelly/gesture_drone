gesture_drone
=============
Last Updated: 16 October 2014

Gesture controlled drone using a wearable device powered by Arduino.

The repository contains all the schematic diagrams and source codes for the drone and wearable device.

HARDWARE
--------------
DRONE
- 1 x Arduino Mega
- 1 x HC-06 (Slave) Bluetooth Breakout Board
- 1 x 9V Battery
- 1 x Drone Kit(Motors, ESC, Flight Controller, Receiver and Transmitter, Li-Po Battery)

WEARABLE
- 1 x Arduino Pro Mini-5v
- 1 x Pololu MinIMU-9 v2 Gyro, Accelerometer, and Compass (L3GD20 and LSM303DLHC Carrier)
- 1 x HC-SR04 Ultrasonic Sensor
- 1 x HC-05 (Master) Bluetooth Breakout Board
- 2 x CR-2302 Battery Holders
- 2 x CR-2302 Battery Cells
- 1 x Prototyping Board
    
    
SOFTWARE
--------------
DRONE
- Upload drone.ino to the Arduino Mega
  
WEARABLE
- Add https://github.com/pololu/lsm303-arduino to your Arduino library
- Upload wearable.ino to the Arduino Pro Mini
    
