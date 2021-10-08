#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h> //temp sensor
#define REPORTING_PERIOD_MS     30000
Adafruit_MLX90614 MLX_Sensor = Adafruit_MLX90614();  //temp sensor
PulseOximeter pox;
uint32_t tsLastReport = 0;
uint32_t tsReport = 0;
//Define variables
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
//uint32_t tsLastReport = 0;
unsigned long cnt_pulse = 0;
int i_mlx = 0;
int i=0;
float temperature, temp;
float sum;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup()
{
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip

    
}
//int i = 0;
void loop()
{
    // Make sure to call update as fast as possible
    cnt = 0;


    //temperature
    //if(dispensed == 1)
   //{
    //Serial.println("Temperature Measuring! ");  

    MLX_Sensor.begin(0x5A);  
    delay(1000);
    Serial.println("Place your hand on Temp sensor");
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
    Serial.println(temperature);
    delay(2000);
    temp_done = 1;
    Serial.println("Temperature Measured Successfully");
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
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    tsLastReport = 0;
    //Serial.print("Heart rate:");
    
    Serial.println(millis());
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
    
    Serial.println(millis());
    //}
    
}
