#pragma once
//
//    FILE: SHT4x.h
//  AUTHOR: Samuel Cuerrier Auclair
//  AUTHOR: Rob Tillaart
// VERSION: 0.0.1
//    DATE: 2025-08-11
// PURPOSE: Arduino library for the SHT4x temperature and humidity sensor. High precision sensor with I2C interface.
//          This is a fork of the SHT31 library by Rob Tillaart, modified to work with the SHT4x series.
//          https://www.adafruit.com/product/2857
//     URL: https://github.com/RobTillaart/SHT4x


#include "Arduino.h"
#include "Wire.h"


#define SHT4x_LIB_VERSION             (F("0.0.1"))

#ifndef SHT_DEFAULT_ADDRESS
#define SHT_DEFAULT_ADDRESS           0x44
#endif

// SHT4x measurement commands
#define SHT4x_MEASUREMENT_SLOW        0xFD
#define SHT4x_MEASUREMENT_MEDIUM      0xF6
#define SHT4x_MEASUREMENT_FAST        0xE0
// SHT4x measurement commands with heater on
#define SHT4x_MEASUREMENT_LONG_HIGH_HEAT    0x39
#define SHT4x_MEASUREMENT_SHORT_HIGH_HEAT   0x32
#define SHT4x_MEASUREMENT_LONG_MEDIUM_HEAT  0x2F
#define SHT4x_MEASUREMENT_SHORT_MEDIUM_HEAT 0x24
#define SHT4x_MEASUREMENT_LONG_LOW_HEAT     0x1E
#define SHT4x_MEASUREMENT_SHORT_LOW_HEAT    0x15

//  error codes
#define SHT4x_OK                      0x00
#define SHT4x_ERR_WRITECMD            0x81
#define SHT4x_ERR_READBYTES           0x82
#define SHT4x_ERR_HEATER_OFF          0x83
#define SHT4x_ERR_NOT_CONNECT         0x84
#define SHT4x_ERR_CRC_TEMP            0x85
#define SHT4x_ERR_CRC_HUM             0x86
#define SHT4x_ERR_CRC_STATUS          0x87
#define SHT4x_ERR_HEATER_COOLDOWN     0x88
#define SHT4x_ERR_HEATER_ON           0x89
#define SHT4x_ERR_SERIAL_NUMBER_CRC   0x8A


class SHT4x
{
public:
  SHT4x(uint8_t address = SHT_DEFAULT_ADDRESS, TwoWire *wire = &Wire);

  bool    begin();
  uint8_t getAddress();

  //  check sensor is reachable over I2C
  virtual bool isConnected();

  //  blocks for the required time and read after
  bool read(uint8_t measurementType = SHT4x_MEASUREMENT_SLOW, bool errorCheck = true);

  //  lastRead is in milliSeconds since start
  uint32_t lastRead() { return _lastRead; };

  bool reset();

  //  0..100%
  float    getHumidity()       { return _rawHumidity    * (125.0 / 65535.0) - 6.0; };
  //  getTemperature returns Celsius
  float    getTemperature()    { return _rawTemperature * (175.0 / 65535.0) - 45.0; };
  float    getFahrenheit()     { return _rawTemperature * (63.0 /13107.0) - 49.0; };
  //  raw data e.g. debugging or efficient logging / transmit.
  uint16_t getRawHumidity()    { return _rawHumidity; };
  uint16_t getRawTemperature() { return _rawTemperature; };


  //  ASYNC INTERFACE
  bool requestData(uint8_t measurementType = SHT4x_MEASUREMENT_SLOW);
  bool dataReady();
  bool readData(bool errorCheck = true);

  //  MISC
  int getError();  //  clears error flag
  //  fast == true, => skips CRC check
  bool getSerialNumber(uint32_t &serial, bool errorCheck = true);


protected:
  uint8_t  _address;
  uint8_t _lastMeasurementType; //  SHT4x_MEASUREMENT_SLOW, SHT4x_MEASUREMENT_MEDIUM, SHT4x_MEASUREMENT_FAST
  uint32_t _lastRead;
  uint32_t _lastRequest;   //  for async interface
  uint16_t _rawHumidity;
  uint16_t _rawTemperature;
  uint8_t  _error;

private:
  uint32_t getDelay();
  bool validateMeasCmd(uint8_t cmd)
  {
    return (cmd == SHT4x_MEASUREMENT_SLOW || cmd == SHT4x_MEASUREMENT_MEDIUM ||
            cmd == SHT4x_MEASUREMENT_FAST || cmd == SHT4x_MEASUREMENT_LONG_HIGH_HEAT ||
            cmd == SHT4x_MEASUREMENT_SHORT_HIGH_HEAT || cmd == SHT4x_MEASUREMENT_LONG_MEDIUM_HEAT ||
            cmd == SHT4x_MEASUREMENT_SHORT_MEDIUM_HEAT || cmd == SHT4x_MEASUREMENT_LONG_LOW_HEAT ||
            cmd == SHT4x_MEASUREMENT_SHORT_LOW_HEAT);
  }
  uint8_t crc8(const uint8_t *data, uint8_t len);
  virtual bool writeCmd(uint8_t cmd);
  virtual bool readBytes(uint8_t n, uint8_t *val);
  TwoWire* _wire;
};


//  -- END OF FILE --