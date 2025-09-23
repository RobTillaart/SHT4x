//
//    FILE: SHT4x_I2Cspeed
//  AUTHOR: Rob Tillaart
// PURPOSE: testing the performance at different I2C speeds
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
  for (uint32_t I2Cfreq = 100000; I2Cfreq < 900000; I2Cfreq += 50000)
  {
    Serial.print(I2Cfreq/1000);
    Wire.setClock(I2Cfreq);
    test();
  }
  Serial.println();
}


void test()
{
  start = micros();
  sht.read(false);         //  default = true/CRC Check       slow = no crc check
  stop = micros();
  Serial.print("\t");
  Serial.print(stop - start);
  Serial.print("\t");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\t");
  Serial.println(sht.getHumidity(), 1);
  delay(1000);
}


//  -- END OF FILE --

