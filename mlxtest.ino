#include <Wire.h>
#include <Adafruit_MLX90614.h>
char *Object_Type[]={"Object","Ambient"};

Adafruit_MLX90614 MLX_Sensor = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("MLX90614 Sensor MicroLab");  
  MLX_Sensor.begin();  
}

void loop() {

  Display_Temperature('A'); //Get Object temperature in Celsius
  Display_Temperature('B'); //Get Ambient temperature in Celsius
  
  Serial.println("########");

  delay(2000);

  
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
