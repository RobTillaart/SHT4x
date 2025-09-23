//
//    FILE: SHT4x_heater.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo heater functions
//     URL: https://github.com/RobTillaart/SHT4x


#include "Wire.h"
#include "SHT4x.h"

#define SHT_DEFAULT_ADDRESS   0x44

SHT4x sht;

void setup()
{
  //  while(!Serial);  //  uncomment if needed
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("SHT4x_LIB_VERSION: \t");
  Serial.println(SHT4x_LIB_VERSION);
  Serial.println();

  Wire.begin();
  Wire.setClock(100000);
  sht.begin();

  // Read initial temperature and humidity
  sht.read();
  Serial.print("Temperature:\t");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\tHumidity:\t");
  Serial.println(sht.getHumidity(), 1);
  Serial.println();
  Serial.println("Temperature should now increase and humidity decrease.");
}


void loop()
{
  sht.read(SHT4x_MEASUREMENT_SHORT_HIGH_HEAT);
  Serial.print("Temperature:\t");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\tHumidity:\t");
  Serial.println(sht.getHumidity(), 1);
  delay(1000); // Heater should not have a duty cycle more than 10% (section 4.9)
}


//  -- END OF FILE --
