// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 7; 
// Create a servo object 
Servo Servo1; 

void setup() { 
   // We need to attach the servo to the used pin number 
   Serial.begin(9600);
   Servo1.attach(servoPin); 
}
void loop(){ 
   // Make servo go to 0 degrees 
   Servo1.write(0); 
   Serial.println("ZERO Degree");
   delay(100); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   Serial.println("90 Degree");
   delay(100); 
   // Make servo go to 180 degrees 
   Servo1.write(180); 
   Serial.println("180 Degree");
   delay(100); 
}
