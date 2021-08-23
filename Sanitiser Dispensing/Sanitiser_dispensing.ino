#define echoPin 2
#define trigPin 3

long duration;
int distance;

// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 7; 
// Create a servo object 
Servo Servo1; 

void setup(){
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Servo1.attach(servoPin);
 }
 
void loop(){

  Serial.println("Place your Hand");
  
  // trigger sent
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  //calculate duration when echo pin is HIGH
  duration=pulseIn(echoPin,HIGH);
  distance=(duration*0.034/2);
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm ");

  //if distance is less than 10cm 
   if (distance < 10)
   {
    delay(1000);
    Serial.println("Hand Detected");
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   Serial.println("ZERO Degree");
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   Serial.println("180 Degree");
   delay(1000); 
   Serial.println("Hand Detected");
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   Serial.println("ZERO Degree");
   delay(1000); 
   }

  delay(1000);   
}
