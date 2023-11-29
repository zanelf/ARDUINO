
#include <Wire.h>
#include "SparkFunISL29125.h"

// Declare sensor object
SFE_ISL29125 RGB_sensor;

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
}

// Read sensor values for each color and print them to serial monitor
void loop()
{
  // Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();

  //red = map(red,0,RGB_sensor());
  int negro= 450;
  int suma=red+green+blue;
  // Print out readings, change HEX to DEC if you prefer decimal output

/*

  if(suma>negro){
    Serial.print("color");
    Serial.println();
    }
  else {
    Serial.print("negro"); 
    Serial.println();
  }

*/
  
 //Serial.print("Red: "); 
 Serial.print(red);
 //Serial.print("Green: "); 
 Serial.print(", ");
 Serial.print(green);
 //Serial.print("Blue: "); 
 Serial.print(", ");
 Serial.println(blue);
 //Serial.println();
  delay(500);
}
