//
//    FILE: SHT4x_isConnected.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo  (needs 0.2.4 or up)
//     URL: https://github.com/RobTillaart/SHT4x


#include "Wire.h"
#include "SHT4x.h"

#define SHT_DEFAULT_ADDRESS   0x44

uint32_t start;
uint32_t stop;

SHT4x sht;  //  use default address and Wire

uint32_t connectionFails = 0;


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
  if ( sht.isConnected()  )
  {
    start = micros();
    bool b = sht.read();         //  default = true/CRC check       no crc check = false
    stop = micros();

    int error = sht.getError();

    Serial.print(millis());
    Serial.print("\t");
    Serial.print(stop - start);
    Serial.print("\t");
    Serial.print(b, HEX);
    Serial.print("\t");
    Serial.print(error, HEX);
    Serial.print("\t");
    Serial.print(sht.getTemperature(), 1);
    Serial.print("\t");
    Serial.print(sht.getHumidity(), 1);
  }
  else
  {
    connectionFails++;
    Serial.print(millis());
    Serial.print("\tNot connected:\t");
    Serial.print(connectionFails);
    // sht.reset();
  }
  Serial.println();
  delay(1000);
}


//  -- END OF FILE --

