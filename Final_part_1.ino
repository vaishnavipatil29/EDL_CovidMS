//#------------Libraries------------------#
#include "MAX30100_PulseOximeter.h"  //pulse oximeter
#include <Servo.h>   //servo motor
#include <Adafruit_MLX90614.h> //temp sensor

//#------------Define Pins----------------#
//Ultrasonic Sensor
#define echoPin 8
#define trigPin 9
//pulse oximeter
#define REPORTING_PERIOD_MS     1000

//# Define variables
//temp sensor
char *Object_Type[]={"Object","Ambient"};
long duration;
int distance;
int servoPin = 7; 
unsigned long cnt = 0;
unsigned long StartTime;
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long dispensed = 0;
unsigned long pulse_oxi = 0;
unsigned long temp_done = 0;
//pulse oxi
uint32_t tsLastReport = 0;
unsigned long cnt_pulse = 0;


//Create objects
Adafruit_MLX90614 MLX_Sensor = Adafruit_MLX90614();  //temp sensor
Servo Servo1; //servo motor
PulseOximeter pox;


// Print Beat when heart rate detected
void onBeatDetected()
{
    Serial.println("Beat!");
}


void setup(){
  Serial.begin(115200);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Servo1.attach(servoPin);
 }
 
void loop(){

  //restoring flag values for every loop
  dispensed = 0;
  temp_done = 0;
  pulse_oxi = 0;

  Serial.println("Welcome to Covid Monitoring System!");
  delay(100);

  Serial.println("Sanitise your hands");
  delay(100);
  Serial.println("Place your Hand in front of sensor");
  
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
  delay(1000);

    //if distance is less than 10cm 
   if (distance < 10)
   {
    
     if(cnt == 1)
     {
      Serial.println("Wait for some time");
      delay(100);
     }
     else
     {
      cnt = 1;
      delay(1000);
      Serial.println("Hand Detected");
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   //Serial.println("ZERO Degree");
   // Make servo go to 90 degrees 
   Servo1.write(90); 
   //Serial.println("180 Degree");
   delay(1000); 
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   //Serial.println("ZERO Degree");
   delay(1000); 
  dispensed = 1;
  Serial.println("Sanitiser Dispensed successfully");
     }
    
   }
   else if (distance > 10)
   {
    cnt = 0;
   }

    //temperature
     if(dispensed == 1)
     {
        
         MLX_Sensor.begin(0x5A);  
         delay(1000);
         Serial.println("Temperature Measuring! ");  
         delay(100);
         Serial.println("Place your hand");
         delay(100);
         Display_Temperature('A'); //Get Object temperature in Celsius
         Display_Temperature('B'); //Get Ambient temperature in Celsius
  
        Serial.println("########");
      
        delay(2000);
        temp_done = 1;
        Serial.println("Temperature Measured Successfully");
        delay(100);

     }
  
}


float Get_Temperature_Sample(char type)
{
    float temp_value;
    float Object_Temperature = MLX_Sensor.readObjectTempC();
    float Ambient_Temperature = MLX_Sensor.readAmbientTempC();
   if(type =='A')
   {
    temp_value = Object_Temperature;
   }else if(type =='B')
   {
    temp_value = Ambient_Temperature;
   }
   return temp_value;
 
}

void Display_Temperature(char type)
{
  float temp_data =Get_Temperature_Sample(type);

  if(type =='A')
  {
    Serial.print(Object_Type[0]);
    Serial.print(" ");    
    Serial.print(temp_data);
    Serial.print("°");      
    Serial.println("C");
  }else if(type =='B')
  {
    Serial.print(Object_Type[1]);
    Serial.print(" ");     
    Serial.print(temp_data);
    Serial.print("°");      
    Serial.println("C");
  }

}
