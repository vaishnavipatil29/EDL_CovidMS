
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
#include <Adafruit_MLX90614.h>
char *Object_Type[]={"Object","Ambient"};

Adafruit_MLX90614 MLX_Sensor = Adafruit_MLX90614();
PulseOximeter pox;
uint32_t tsLastReport = 0;
unsigned long cnt = 0;


//int address_sensor1= 87; //pulse oximeter

//int address_sensor2= 90; //temperature

void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200); //this creates the Serial Monitor
delay(1000);
 
}

void loop() {
  // put your main code here, to run repeatedly:
   MLX_Sensor.begin(0x5A);  

        Serial.println("MLX90614 Sensor MicroLab");  
         Display_Temperature('A'); //Get Object temperature in Celsius
         Display_Temperature('B'); //Get Ambient temperature in Celsius
  
        Serial.println("########");
      
        delay(2000);
         if (!pox.begin(0x57)) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

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
