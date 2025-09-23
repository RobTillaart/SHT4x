//
//    FILE: SHT4x_lastRead.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo lastRead
//     URL: https://github.com/RobTillaart/SHT4x


#include "Wire.h"
#include "SHT4x.h"

#define SHT_DEFAULT_ADDRESS   0x44

uint32_t start;
uint32_t stop;

SHT4x sht(SHT_DEFAULT_ADDRESS, &Wire);  //  use explicit address and Wire


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
  sht.read();         //  default = true/CRC check       no crc check = false
  Serial.print("\t");
  Serial.print(sht.lastRead());
  Serial.print("\t");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\t");
  Serial.println(sht.getHumidity(), 1);
  delay(100);
}


//  -- END OF FILE --

