//
//    FILE: SHT4x_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/SHT4x


#include "Wire.h"
#include "SHT4x.h"

#define SHT_DEFAULT_ADDRESS   0x44

uint32_t start;
uint32_t stop;

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
}


void loop()
{
  start = micros();
  sht.read();         // default SHT4x_MEASUREMENT_SLOW and true for CRC check
  stop = micros();

  Serial.print("\t");
  Serial.print(stop - start);
  Serial.print("\t");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\t");
  Serial.println(sht.getHumidity(), 1);
  delay(100);
}


//  -- END OF FILE --

