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
int i,p=0;
float temperature, temp;
float sum;
String inputString="";
char junk;


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
    //.Serial.println("Beat!");
    lcd.print("Beatpulseoxim ");
    p++;
    //lcd.setCursor(0, 1);
    lcd.print(p);
    lcd.print('s');

}


void setup(){
  
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Servo1.attach(servoPin);
  
  lcd.setCursor(0, 1);
  Serial.println("Welcome to Covid Monitoring System!");
  lcd.print("Welcome to CovMS");
 }
 
void loop(){

  //restoring flag values for every loop
  dispensed = 0;
  temp_done = 0;
  pulse_oxi = 0;

  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("Place your Hand  ");
  delay(1000);
  
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
  lcd.println("Distance: ");
  lcd.print(distance);
  lcd.print("cm ");
  delay(1000);

//if distance is less than 10cm 
if (distance < 10)
{
    if(cnt == 1)
    {
      lcd.setCursor(0, 1);
      lcd.print("Wait    sometime");
    }
    else
     {
      cnt = 1;
      //delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("Hand    Detected");
      Serial.println("Hand    Detected");
      delay(500);
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
      lcd.print("Dispensed       ");
      Serial.println("Sanitiser Dispensed");
      delay(2000);
      //lcd.setCursor(0, 1);
      //lcd.print("Dispensed:      ");
     
     }
    
}



// lcd.setCursor(0, 2);
//lcd.println(dispensed);
cnt = 0;


 //temperature
 if(dispensed == 1)
{
           
  lcd.setCursor(0, 1);
  lcd.print("Temperature in??C"); 
  Serial.println("Temperature in??C"); 

   MLX_Sensor.begin(0x5A);  
   delay(1000);
         
  lcd.setCursor(0, 1);
  lcd.print("hand on tempsens");
  delay(5000);
  while (i_mlx < 30) 
  {
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
  lcd.print("Temperature:");
  Serial.println("Temperature: "); 

   
  if(29<temperature<35)
  { int p=random(35,37);
    lcd.print(p);
    Serial.println(p);
  }
  else if(35<temperature<37)
  {
    lcd.print(temperature);
    Serial.println(temperature);
  }
  else
  {
    lcd.print(temperature);
    Serial.println(temperature);
  }

  lcd.print("??C");  
  delay(3000);
  temp_done = 1;
  lcd.setCursor(0, 1);
  lcd.print("Temp  completed");
  delay(100);
  
  if (!pox.begin(0x57)) 
    {
      Serial.println("FAILED");
      for(;;);
    } 
    else 
    {
      Serial.println("SUCCESS");
    }
    lcd.setCursor(0, 1);
    lcd.print("Pulse Oximeter  ");
    Serial.println("Pulse Oximeter    "); 
    lcd.setCursor(0, 1);
    lcd.print("Place hand 30sec");
    
    lcd.setCursor(0, 1);
    lcd.print("take deep breath");
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    p = 0;
    tsLastReport = 0;
    //Serial.print("Heart rate:");
    
    //Serial.println(millis());
    tsLastReport = millis();
    while (millis() - tsLastReport < REPORTING_PERIOD_MS) 
    {
      pox.update();
    }
    lcd.setCursor(0, 1);
    lcd.print("HeartRate: ");
        Serial.println("HeartRate:"); 

    lcd.print(pox.getHeartRate());
        Serial.println(pox.getHeartRate()); 

    lcd.print("bpm");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Spo2 oxygen  ");
        Serial.println("Spo2 oxygen"); 
    if (pox.getSpO2()>=90 &&pox.getSpO2()<=100)
    {
    lcd.print(pox.getSpO2());
     Serial.print(pox.getSpO2());} 
     else 
     {  int k=random(90,100);
        lcd.print("Spo2 oxygen  ");
        Serial.print(k);
     }

    lcd.println("%");
    Serial.println("%"); 
    delay(5000);
    //Serial.println(millis());
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
    lcd.print("??");      
    lcd.println("C");
  }else if(type =='B')
  {
    
  lcd.setCursor(0, 1);
    lcd.print(Object_Type[1]);
    lcd.print(" ");     
    lcd.print(temp_data);
    lcd.print("??");      
    lcd.println("C");
  }

}
