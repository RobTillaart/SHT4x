//
//    FILE: SHT4x_two_I2C_array.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo multiple devices in array
//     URL: https://github.com/RobTillaart/SHT4x
//
//   NOTE: see issue #22 for details
//         originally written for a ATSAMD21G18A custom board.
//         edited for a board (e.g. ESP32) that has Wire and Wire1 (compiles, not tested)


#include "Wire.h"
#include "SHT4x.h"


TwoWire myWire(&sercom5, 0, 1);
//  TwoWire myWire = Wire1;


SHT4x sht[4] = {
  SHT4x(0x44, &Wire),
  SHT4x(0x45, &Wire),
  SHT4x(0x44, &myWire),
  SHT4x(0x45, &myWire)
};

bool b[4];


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
  myWire.begin();
  myWire.setClock(100000);

  //  see datasheet for details
  pinPeripheral(0, PIO_SERCOM_ALT);
  pinPeripheral(1, PIO_SERCOM_ALT);

  //  show they are connected
  Serial.print("BEGIN:\t");
  for (uint8_t i = 0; i < 4; i++)
  {
    b[i] = sht[i].begin();
    Serial.print(b[i]);
    Serial.print("\t");
  }
  Serial.println();
}


void loop()
{
  //  request data on all that are found
  for (uint8_t i = 0; i < 4; i++)
  {
    if (b[i]) sht[i].requestData();
  }

  // wait for all data to be ready
  for (uint8_t i = 0; i < 4; i++)
  {
    if (b[i]) while(!sht[i].dataReady()) yield();
  }

  for (uint8_t i = 0; i < 4; i++)
  {
    Serial.print(sht[i].getTemperature(), 1);
    Serial.print("\t");
  }
  for (uint8_t i = 0; i < 4; i++)
  {
    Serial.print(sht[i].getHumidity(), 1);
    Serial.print("\t");
  }
  Serial.println();

  delay(1000);
}


//  -- END OF FILE --
