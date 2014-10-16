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

int recAIL;
int recELE;
int recTHR;
int recRUD;
int recAUX;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  AIL.attach(9);
  ELE.attach(10);
  THR.attach(11);
  RUD.attach(12);
  delay(500);
}

void loop() {

  recAIL = pulseIn(3, HIGH, 20000);
  recELE = pulseIn(4, HIGH, 20000);
  recTHR = pulseIn(5, HIGH, 20000);
  recRUD = pulseIn(6, HIGH, 20000);
  recAUX = pulseIn(7, HIGH, 20000);

  AIL.writeMicroseconds(recAIL);
  ELE.writeMicroseconds(recELE);
  THR.writeMicroseconds(recTHR);
  RUD.writeMicroseconds(recRUD);

  Serial.print("AIL: ");
  Serial.print(recAIL);
  Serial.print('\t');
  Serial.print("ELE: ");
  Serial.print(recELE);
  Serial.print('\t');
  Serial.print("THR: ");
  Serial.print(recTHR);
  Serial.print('\t');
  Serial.print("RUD: ");
  Serial.print(recRUD);
  Serial.print('\t');
  Serial.print("AUX: ");
  Serial.print(recAUX);
  Serial.print('\n');
}






