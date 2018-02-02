#include <Servo.h>
#include <Tone.h>
#include "pitches.h" 
#include "IO_pins.h"

// define global variables

int leftmotorstop=1470;
int rightmotorstop=1500;
int neckLRcenter=1500;
int neckUDcenter=1500;

int LRmax=neckLRcenter+700;
int LRmin=neckLRcenter-700;
int UDmax=neckUDcenter+700;
int UDmin=neckUDcenter-200;

byte LRscalefactor=3;
byte UDscalefactor=3;
int distancemax=180;
int bestdistance=550;

long time;
byte boredom;
int pan=neckLRcenter;
int tilt=neckUDcenter;
int panscale;
int tiltscale;
int leftspeed=leftmotorstop;
int rightspeed=rightmotorstop;
byte lightchase;
byte edge;
byte edgesensors;
int distance;
int temp;
int updown;
int leftright;
int leftIRvalue;
int rightIRvalue;
int upIRvalue;
int downIRvalue;
int safedistance=200;
int leftfrontsen;
int leftrearsen;
int rightrearsen;
int rightfrontsen;
int pos = 0;

Servo leftmotor;
Servo rightmotor;
Servo neckleftright;
Servo neckupdown;


void setup()
{
  // initialize servos and configure pins
  leftmotor.attach(leftmotorpin);
  leftmotor.writeMicroseconds(leftmotorstop);
  rightmotor.attach(rightmotorpin);
  rightmotor.writeMicroseconds(rightmotorstop);
  neckleftright.attach(neckleftrightpin);
  neckleftright.writeMicroseconds(neckLRcenter);
  neckupdown.attach(neckupdownpin);
  neckupdown.writeMicroseconds(neckUDcenter);

  pinMode (IRleds,OUTPUT);
  pinMode (Speaker,OUTPUT);
  pinMode (leftfrontLED,OUTPUT);
  pinMode (leftrearLED,OUTPUT);
  pinMode (rightrearLED,OUTPUT);
  pinMode (rightfrontLED,OUTPUT);

  // play tune on powerup / reset
  int melody[] = {NOTE_C4,NOTE_G3,NOTE_G3,NOTE_A3,NOTE_G3,0,NOTE_B3,NOTE_C4};
  int noteDurations[] = {4,8,8,4,4,4,4,4};
  for (byte Note = 0; Note < 8; Note++)
  {
    int noteDuration = 1000/noteDurations[Note];
    tone(Speaker,melody[Note],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}

void loop()
{
  if (millis()-time>199)
  {
    time=millis();
    lightchase=lightchase+1-4*(lightchase>3);
  }
  leftmotor.writeMicroseconds(leftspeed);
  rightmotor.writeMicroseconds(rightspeed);
  neckleftright.writeMicroseconds(pan);
  neckupdown.writeMicroseconds(tilt);

  
  ObjectDetection();
  ReverseLeft();
  ReverseRight();
  Drive();

}

byte noise; 

//============================================================Avoid hitting objects==================================================

void ObjectDetection()
{
  //turn on edge detection LEDs
  digitalWrite(leftfrontLED,1);
  digitalWrite(leftrearLED,1);
  digitalWrite(rightrearLED,1);
  digitalWrite(rightfrontLED,1);
  delayMicroseconds(100);                                     
  
  // read IR sensors
  leftfrontsen=digitalRead(leftfrontsenpin);
  leftrearsen=digitalRead(leftrearsenpin);
  rightrearsen=digitalRead(rightrearsenpin);
  rightfrontsen=digitalRead(rightfrontsenpin);

  // turn off edge detection LEDs
  digitalWrite(leftfrontLED,0);
  digitalWrite(leftrearLED,0);
  digitalWrite(rightrearLED,0);
  digitalWrite(rightfrontLED,0);
  delayMicroseconds(100);                                     

  // turn on indicator LED if edge is detected otherwise chase LEDs
  digitalWrite(leftfrontLED,(lightchase==1 || leftfrontsen==0));
  digitalWrite(leftrearLED,(lightchase==2 || leftrearsen==0));
  digitalWrite(rightrearLED,(lightchase==3 || rightrearsen==0));
  digitalWrite(rightfrontLED,(lightchase==4 || rightfrontsen==0));

  // Stop motor to prevent falling over the edge
  if (leftrearsen==0 && leftspeed>leftmotorstop) leftspeed=leftmotorstop;
  if (rightrearsen==0 && rightspeed<rightmotorstop) rightspeed=rightmotorstop;
  
}

void ReverseLeft()
{
  if (leftfrontsen==0) {for(pos = 0; pos < 180; pos += 1); leftmotor.write(pos); delay(2000); }

  
}

void ReverseRight()
{
  if (rightfrontsen==0) {for(pos = 180; pos >= 1; pos -= 1); rightmotor.write(pos); delay(2000); }
}

void Drive()
{
  rightspeed=rightmotorstop+60+temp;
  leftspeed=leftmotorstop-60-temp;
}














