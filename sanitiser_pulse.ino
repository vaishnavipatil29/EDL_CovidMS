#define echoPin 8
#define trigPin 9
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
#include <Adafruit_MLX90614.h>
char *Object_Type[]={"Object","Ambient"};

Adafruit_MLX90614 MLX_Sensor = Adafruit_MLX90614();


long duration;
int distance;

// Include the Servo library 
#include <Servo.h> 
// include the library code:
//#include <LiquidCrystal.h>
// Declare the Servo pin 
int servoPin = 7; 
// Create a servo object 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo Servo1; 
unsigned long cnt = 0;
unsigned long StartTime;
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long dispensed = 0;
unsigned long pulse_oxi = 0;
unsigned long temp_done = 0;

PulseOximeter pox;
uint32_t tsLastReport = 0;
unsigned long cnt_pulse = 0;

void onBeatDetected()
{
    Serial.println("Beat!");
}
void setup(){
  //Serial.begin(9600);
  Serial.begin(115200);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Servo1.attach(servoPin);
//  lcd.begin(16, 2);
  //lcd.print("SANITISER DISPENSOR");
  
  //unsigned long StartTime = 0;
  //millis();

    //Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

      MLX_Sensor.begin();  


 }
 
void loop(char type){

  //restoring flag values for every loop
  dispensed = 0;
  temp_done = 0;
  pulse_oxi = 0;
  
  Serial.println("Place your Hand");
  //lcd.setCursor(0, 1);
  //lcd.print("Place your Hand");
  
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
     }
     else
     {
      cnt = 1;
//    lcd.setCursor(0, 1);
 //   lcd.print("HAND DETECTED");
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
  // lcd.setCursor(0, 1);
  // lcd.print("DISPENSED!");
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
        Serial.println("MLX90614 Sensor MicroLab");  
         Display_Temperature('A'); //Get Object temperature in Celsius
         Display_Temperature('B'); //Get Ambient temperature in Celsius
  
        Serial.println("########");
      
        delay(2000);
        temp_done = 1;

     }

   //pulse
    // Make sure to call update as fast as possible
    
    if(temp_done == 1)
    {
    while(pulse_oxi == 0)
    {
      pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

        cnt = cnt + 1;
        if(cnt >6)
        {
          // if(pox.getHeartRate()>0 and pox.getSpO2() >0)
        //{
        
        
          Serial.print("Heart rate:");
          Serial.print(pox.getHeartRate());
            Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        pulse_oxi = 1;
          
        //}
        //else
        //{
        //  Serial.println("FInding...Take Deep breathe");
        //}
    
        }
        else
        {
          Serial.println("FInding...Take Deep breathe");
          pox.getHeartRate();
          pox.getSpO2();
        }
       
       
       Serial.println("%");
 
        tsLastReport = millis();
      
    }
    
     
    }
    }

    


  delay(1000);   
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

  
