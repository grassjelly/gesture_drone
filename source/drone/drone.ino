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
#include <Servo.h>
#define LED 13

Servo AIL;
Servo ELE;
Servo THR;
Servo RUD;
Servo AUX;

int rec_ail;
int rec_ele;
int rec_thr;
int rec_rud;
int rec_aux;

int max_rec;
int min_rec;
int mid_rec;
int ref_aux;
int ele, ail, thr, rud;
int x,y,z;

String Axes = ""; 
int cal_height;

int XMIT_THRESH = 70;
int WRBL_THRESH = 50;

void setup() {

  Serial.begin(9600); //debugging Serial port baudrate
  Serial1.begin(9600); //BT transmission Serial port baudrate
  pinMode(9, OUTPUT); //Flight controller - Aileron Pin
  pinMode(10, OUTPUT); //Flight controller - Elevator Pin
  pinMode(11, OUTPUT); //Flight controller - Throttle Pin
  pinMode(12, OUTPUT); //Flight controller - Rudder Pin
  pinMode(13, OUTPUT); //LED pin
  AIL.attach(9); //Servo settings
  ELE.attach(10);
  THR.attach(11);
  RUD.attach(12);
  rec_ail = pulseIn(3, HIGH, 20000); // Take 10 samples of receiver readings
  for (int i = 0; i < 10; i++) {
    mid_rec += rec_ail;
  }
  ref_aux = pulseIn(7, HIGH, 20000); //calculate reference aux value
  mid_rec = mid_rec / 10; //calculate average receiver's mid-stick values
  min_rec = mid_rec - 450;//calculate receiver's min-stick values
  max_rec = mid_rec + 450;//calculate receiver's max-stick values

}


void loop() {
  // read pwm values from the receiver
  rec_ail = pulseIn(3, HIGH, 20000);
  rec_ele = pulseIn(4, HIGH, 20000);
  rec_thr = pulseIn(5, HIGH, 20000);
  rec_rud = pulseIn(6, HIGH, 20000);
  rec_aux = pulseIn(7, HIGH, 20000);

  //check if aux switch is flipped. if so enable wearable control
  if(rec_aux > 0 && rec_aux < (ref_aux - 400)  ){
    digitalWrite(LED,HIGH); //Indicates that wearable is enabled 
    read_wearable(); //call wearable BT data parser function
    rud = mid_rec; //lock rudder to mid-stick

    //If there's any input from the safety pilot on Aileron, Elevator, Throttle or Rudder, enable the main transmitter to control the drone
    if(rec_ail > mid_rec + XMIT_THRESH || rec_ail < mid_rec - XMIT_THRESH){
      ail = rec_ail;
    }
    if(rec_ele > mid_rec + XMIT_THRESH || rec_ele < mid_rec - XMIT_THRESH){
      ele = rec_ele;
    }
    if(rec_thr > min_rec + XMIT_THRESH){
      thr = rec_thr;
    }
    if(rec_rud > mid_rec + XMIT_THRESH || rec_rud < mid_rec - XMIT_THRESH){
      rud = rec_rud;
    }
  }

  else{
    //else, pass remote control signals to the flight controller
    digitalWrite(LED, LOW);//Indicates that wearable is disabled
    ail = rec_ail;
    ele = rec_ele;
    thr = rec_thr;
    rud = rec_rud;
  }
  // write pwm values to the flight controller
  AIL.writeMicroseconds(ail);
  ELE.writeMicroseconds(ele);
  THR.writeMicroseconds(thr);
  RUD.writeMicroseconds(rud);
  
  print_transmit();

}

void read_wearable(){
  Serial1.print(1);
  while (Serial1.available())
  {
    char data = Serial1.read(); // receive a character from BT port
    Axes.concat(data); // add the received character to buffer 'Axes'
    if (data == 'x') //if delimeter 'x' is read, assign x-axis value
    {
      x = Axes.toInt();
      Axes = "";
    }
    else if (data == 'y') //if delimeter 'y' is read, assign y-axis value
    {
      y = Axes.toInt();
      Axes = "";
    }

    else if (data == 'z') //if delimeter 'z' is read, assign z-axis value
    {
      z = Axes.toInt();
      Axes = "";
    }
  }

  if(x > WRBL_THRESH){//if the wearable is tilted to the right, add correlated value to Aileron's default PWM signal.
    ail = mid_rec + x;
    thr = mid_rec;
  }

  else if (x < -WRBL_THRESH){//if the wearable is tilted to the left, subtract correlated value to Aileron's default PWM signal.
    x = abs(x);
    ail = mid_rec - x;
    thr = mid_rec;
  }

  else if( x > -WRBL_THRESH && x < WRBL_THRESH){//else, Aileron is equals to defualt Aileron PWM signal.
    ail = mid_rec;
  }

  if(y > WRBL_THRESH){//if the wearable is tilted to the front, add correlated value to Elevators's default PWM signal.
    ele = mid_rec + y;
    thr = mid_rec;
  }
  
  else if(y < -WRBL_THRESH){//if the wearable is tilted to the rear, subtract correlated value to Aileron's default PWM signal.
    y = abs(y);
    ele = mid_rec - y;
    thr = mid_rec;
  }
  else if(y > -WRBL_THRESH && y < WRBL_THRESH){//else, Elevator is equals to defualt Elevator PWM signal.
    ele = mid_rec;
  }

  if( (x > -WRBL_THRESH && x < WRBL_THRESH) || (y > -WRBL_THRESH && y < WRBL_THRESH ) ){ //if the wearable is not tilted to either X or Y axis, get correlated Throttle value.
    calc_thr();
  } 

}

void print_axis(){ //function to print received values from the wearable
  Serial.print(x);
  Serial.print('\t');
  Serial.print(y);
  Serial.print('\t');
  Serial.println(z); 
}

void print_transmit(){ //function to print calculated PWM signals to be written over the flight controller
  Serial.print(ail);
  Serial.print('\t');
  Serial.print(ele);
  Serial.print('\t');
  Serial.println(thr);
}

void calc_thr(){
  if(z > 5 && z < 45){ //Height is directly correlated to throttle (This is also used for landing)
    thr = map(z, 5 , 45, min_rec, mid_rec);
  }
  else if( z > 45 && z < 80){ //This height is where the user flies the drone
    thr = mid_rec;
  }
  else if( z > 80 ){//When hand's height is more than 80cm,tell the drone to take-off.
    thr = mid_rec + 100;
  }
}
