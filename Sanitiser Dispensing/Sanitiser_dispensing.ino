
#define echoPin 8
#define trigPin 9

long duration;
int distance;

// Include the Servo library 
#include <Servo.h> 
// include the library code:
#include <LiquidCrystal.h>
// Declare the Servo pin 
int servoPin = 7; 
// Create a servo object 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo Servo1; 
unsigned long cnt = 0;
unsigned long StartTime;
unsigned long CurrentTime;
unsigned long ElapsedTime;

void setup(){
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Servo1.attach(servoPin);
  lcd.begin(16, 2);
  lcd.print("SANITISER DISPENSOR");
  
  //unsigned long StartTime = 0;
  //millis();

 }
 
void loop(){

  Serial.println("Place your Hand");
  lcd.setCursor(0, 1);
  lcd.print("Place your Hand");
  
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
    
     if(cnt == 1)
     {
      Serial.println("Wait for some time");
     }
     else
     {
      cnt = 1;
    lcd.setCursor(0, 1);
    lcd.print("HAND DETECTED");
    delay(1000);
    Serial.println("Hand Detected");
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   //Serial.println("ZERO Degree");
   //delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   Serial.println("180 Degree");
   delay(1000); 
   Serial.println("Hand Detected");
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   Serial.println("ZERO Degree");
   delay(1000); 
   lcd.setCursor(0, 1);
   lcd.print("DISPENSED!");
     }
    
    
   }
   else if (distance > 10)
   {
    cnt = 0;
   }

  delay(1000);   
}
