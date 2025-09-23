//
//    FILE: SHT4x_async.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo async interface
//     URL: https://github.com/RobTillaart/SHT4x


#include "Wire.h"
#include "SHT4x.h"

#define SHT_DEFAULT_ADDRESS   0x44

uint32_t start;
uint32_t stop;
uint32_t cnt;

SHT4x sht;  //  use default address and Wire


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

  sht.requestData();
  cnt = 0;
}


void loop()
{
  if (sht.dataReady())
  {
    start = micros();
    bool success  = sht.readData();   //  default = true for CRC check
    stop = micros();
    sht.requestData();                //  default SHT4x_MEASUREMENT_SLOW

    Serial.print("\t");
    Serial.print(stop - start);
    Serial.print("\t");
    if (success == false)
    {
      Serial.println("Failed read");
    }
    else
    {
      Serial.print(sht.getTemperature(), 1);
      Serial.print("\t");
      Serial.print(sht.getHumidity(), 1);
      Serial.print("\t");
      Serial.println(cnt);
      cnt = 0;
    }
  }
  cnt++;  //  simulate other activity
}


//  -- END OF FILE --

