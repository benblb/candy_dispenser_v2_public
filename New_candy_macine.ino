#include "CTBot.h"
// Include the AccelStepper Library
#include <AccelStepper.h>

/* *** Pinout ***
 *  Stepper steps: D1
 *  Stepper direction: D2
 *  Sonar trig: D5
 *  Sonar echo: D6
 *  LED signal line1: D4
 *  LED signal line2: D8
 *  Photoresistor: A0
*/

//Stepper motor
const int stepPin = D1; // Stepper motor steps
const int dirPin = D2; // Stepper motor direction
#define motorInterfaceType 1 // Define motor interface type (1 for driver)
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

// Set motor rotation tries per dispense
const int rotations = 1;

//Sonar
const int trigPin = D5;
const int echoPin = D6;
float duration, distance; 
int handDist = 10;

bool covered;

// Photoresistor
const int photoPin = A0;

// LED signal
const int led1 = D4;
const int led2 = D8;


void setup() {

  Serial.begin(115200);

  // Stepper setup
  myStepper.setMaxSpeed(1000);
  myStepper.setAcceleration(500);
  myStepper.setSpeed(200); // Steps per second, 200 is one full rotation

  // LED
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

 digitalWrite(led1, LOW);
 digitalWrite(led2, LOW);

  // Sonar
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  covered = false;

Serial.print("Setup complete");
Serial.print("\n");
} 

void loop(){

 // LED waiting flash
 digitalWrite(led1, HIGH);
 digitalWrite(led2, HIGH);


 // Turn motor to dispense candy if hand is close enough to sensor. If sensor is blocked, wait
 if(sonarPulse() && covered == false)
 {
  covered = true;
  
    // LED dispensing flash
   digitalWrite(led1, LOW);
   digitalWrite(led2, HIGH);

   // Dispense
   stepForwards();

   // Set candy in consistent position
   setCandy();

   // Prevent over-dispensing
   delay(1500);
  }
}

bool sonarPulse()
{
 // Sonar pulse
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW); 

 duration = pulseIn(echoPin, HIGH); 
 distance = (duration*.0343)/2;
 if(distance <= handDist)
 {
  return true;
 }
 else
 {
  covered = false;
  return false;
 }
}

// Rotation forwards
void stepForwards()
{
  int steps = myStepper.currentPosition() - 450;
  myStepper.moveTo(steps); // Sets the position +100 steps from current position
  while(myStepper.distanceToGo() != 0)
  {
    myStepper.run();
    yield();
  }
}

// Rotation backwards
void stepBackwards()
{
  int steps = myStepper.currentPosition() + 200;
  myStepper.moveTo(steps); // Sets the position +100 steps from current position
  while(myStepper.distanceToGo() != 0)
  {
    myStepper.run();
    yield();
  }
}

void setCandy()
{
  int steps = myStepper.currentPosition() + 50;
  myStepper.moveTo(steps); // Sets the position +50 steps from current position
  while(myStepper.distanceToGo() != 0)
  {
    myStepper.run();
    yield();
  }
}
