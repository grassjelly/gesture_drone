/*
The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Last Updated: 16 October 2014
Written by; Juan Jimeno
############################################################################################
*/

#include <SoftwareSerial.h>

#include <Wire.h>
#include <LSM303.h>
LSM303 compass;
LSM303::vector<int16_t> running_min = {
  -1383, -2061, -3033}
, running_max = {
  2229, 1974, 1206};

#define triggerZ 9 //Ultrasonic Trigger Pin
#define echoZ 8 //Ultrasonic Echo Pin

SoftwareSerial mySerial(2, 3); // RX, TX
int cx, cy;
unsigned long prevTime= millis();
unsigned long currTime;
unsigned long elapTime;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(triggerZ, OUTPUT);
  pinMode(echoZ, INPUT);
  Wire.begin();
  compass.init();
  compass.enableDefault();


}
void loop() {
  while(mySerial.available()){ //wait for the drone to send "1" before sending all the sensor readings
    if(mySerial.read() == 49){ 
      mySerial.print(calc_x());//send X value through BT serial port
      mySerial.print('x');//X delimiter
      mySerial.print(calc_y());//send Y value through BT serial port
      mySerial.print('y');//Y delimiter
      mySerial.print(calc_z());//send Z value through BT serial port
      mySerial.print('z');//Z delimiter
    }
  }
  //print_raw();
}


int calc_x(){ //read accelerometer's X reading
  compass.read();
  int x;
  x = compass.a.y;
  return(map(x, -16500, 16500, -250, 250));
  
}

int calc_y(){ //read accelerometer's Y reading
  compass.read();
  int y;
  y = compass.a.x;
  return(map(y, -16500, 16500, -250, 250));
}

int calc_z() {//read ultrasonic's Z reading in cm, capped in 200cm.
    long duration, distance;
    digitalWrite(triggerZ, LOW);  
    delayMicroseconds(2); 
    digitalWrite(triggerZ, HIGH);
    delayMicroseconds(10); 
    digitalWrite(triggerZ, LOW);
    duration = pulseIn(echoZ, HIGH, 15000);
    distance = (duration / 2) / 29.1;
    if (duration >11640){
      distance = 200;
    }
    prevTime = millis();
    return (distance);
}

void print_raw(){//print all sensor readings
  Serial.print(calc_x());
  Serial.print('\t');
  Serial.print(calc_y());
  Serial.print('\t');
  Serial.println(calc_z());
}





