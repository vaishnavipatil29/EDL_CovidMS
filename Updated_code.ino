//#------------Libraries------------------#
#include "MAX30100_PulseOximeter.h"  //pulse oximeter
#include <Servo.h>   //servo motor
#include <Adafruit_MLX90614.h> //temp sensor

//#------------Define Pins----------------#
//Ultrasonic Sensor
#define echoPin 8
#define trigPin 9
//pulse oximeter
#define REPORTING_PERIOD_MS     30000  

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
int i_mlx = 0;
int i=0;
float temperature, temp;
float sum;

#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Create objects
Adafruit_MLX90614 MLX_Sensor = Adafruit_MLX90614();  //temp sensor
Servo Servo1; //servo motor
PulseOximeter pox;


// Print Beat when heart rate detected
void onBeatDetected()
{
    
    lcd.setCursor(0, 1);
    Serial.println("Beat!");
        lcd.print("Beat!");

}


void setup(){
  
  Serial.begin(115200);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Servo1.attach(servoPin);
  
  lcd.setCursor(0, 1);
  Serial.print("Welcome to Covid Monitoring System!");
  lcd.print("Welcome to Covid Monitoring System!");
 }
 
void loop(){

  //restoring flag values for every loop
  dispensed = 0;
  temp_done = 0;
  pulse_oxi = 0;

  
  delay(1000);


  
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
  
  lcd.setCursor(0, 1);
  lcd.print("Distance : ");
  

  lcd.print(distance);
  
//  lcd.setCursor(0, 3);
  lcd.print(" cm ");
  delay(1000);

    //if distance is less than 10cm 
   if (distance < 10)
   {
    
     if(cnt == 1)
     {
      
      lcd.setCursor(0, 1);
      lcd.print("Wait for some time");
     }
     else
     {
      cnt = 1;

    delay(1000);
    
    lcd.setCursor(0, 1);
    lcd.print("Hand Detected");
      delay(1000);

    // Make servo go to 0 degrees 
   Servo1.write(0); 
   //Serial.println("ZERO Degree");
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo1.write(90); 
  // Serial.println("180 Degree");
  delay(1000); 
    // Make servo go to 0 degrees 
   Servo1.write(0); 
   //Serial.println("ZERO Degree");
   //delay(1000); 

  dispensed = 1;
  
  lcd.setCursor(0, 1);
  lcd.print("Sanitiser Dispensed successfully");
  delay(1000);

     }
   }

lcd.setCursor(0, 1);
lcd.print("Dispensed: ");

// lcd.setCursor(0, 2);
lcd.println(dispensed);
cnt = 0;


 //temperature
 if(dispensed == 1)
{
           
  lcd.setCursor(0, 1);
           lcd.print("Temperature Measuring! ");  

   MLX_Sensor.begin(0x5A);  
         delay(1000);
         
  lcd.setCursor(0, 1);
         lcd.print("Place your hand on Temp sensor");
         delay(5000);

     while (i_mlx < 30) {
    temp= MLX_Sensor.readObjectTempC();
    sum += temp;
    i_mlx++;
    delay(1);
  }
  temperature = sum/30.0;
  if(temperature < 35)
  {
    //Serial.println("Entered");
    temperature = temperature + 2;
  }
 
  sum = 0;
  i_mlx = 0;
  
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  delay(2000);
  temp_done = 1;
  
  lcd.setCursor(0, 1);
  lcd.print("Temperature Measured Successfully");
  delay(100);
  
}


//Pulse oximeter
   if(dispensed == 1)         
    {
 if (!pox.begin(0x57)) 
    {
      
  lcd.setCursor(0, 1);
      lcd.print("FAILED");
      for(;;);
    } 
    else 
    {
      
  //lcd.setCursor(0, 1);
      lcd.print("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    tsLastReport = 0;
    //Serial.print("Heart rate:");
    
  //lcd.setCursor(0, 1);
    lcd.print(millis());
    tsLastReport = millis();
    while (millis() - tsLastReport < REPORTING_PERIOD_MS) 
    {
      pox.update();
    }
    //if(millis() - tsLastReport > REPORTING_PERIOD_MS)
      //{
      Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
  lcd.setCursor(0, 0);
    lcd.print("Heart rate:");
    
  lcd.setCursor(0, 1);
    lcd.print(pox.getHeartRate());
    
  lcd.setCursor(0, 0);
    lcd.print("bpm / SpO2:");
    
  lcd.setCursor(0, 1);
    lcd.print(pox.getSpO2());
    
  //lcd.setCursor(0, 3);
    //lcd.println("%");
    
  //lcd.setCursor(0, 1);
   // lcd.print(millis());
    //}
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
    
  lcd.setCursor(0, 1);
    lcd.print(Object_Type[0]);
    
    lcd.print(" ");    
    lcd.print(temp_data);
    lcd.print("°");      
    lcd.println("C");
  }else if(type =='B')
  {
    
  lcd.setCursor(0, 1);
    lcd.print(Object_Type[1]);
    lcd.print(" ");     
    lcd.print(temp_data);
    lcd.print("°");      
    lcd.println("C");
  }

}
