#pragma once
//
//    FILE: SHT4x.h
//  AUTHOR: Samuel Cuerrier Auclair
//  AUTHOR: Rob Tillaart
// VERSION: 0.0.3
//    DATE: 2025-08-11
// PURPOSE: Arduino library for the SHT4x temperature and humidity sensor. High precision sensor with I2C interface.
//          This is a fork of the SHT31 library by Rob Tillaart, modified to work with the SHT4x series.
//          https://www.adafruit.com/product/2857
//     URL: https://github.com/RobTillaart/SHT4x


#include "Arduino.h"
#include "Wire.h"


#define SHT4x_LIB_VERSION                   (F("0.0.3"))

#ifndef SHT_DEFAULT_ADDRESS
#define SHT_DEFAULT_ADDRESS                 0x44
#endif

// SHT4x measurement commands
#define SHT4x_MEASUREMENT_SLOW              0xFD
#define SHT4x_MEASUREMENT_MEDIUM            0xF6
#define SHT4x_MEASUREMENT_FAST              0xE0
// SHT4x measurement commands with heater on
#define SHT4x_MEASUREMENT_LONG_HIGH_HEAT    0x39
#define SHT4x_MEASUREMENT_SHORT_HIGH_HEAT   0x32
#define SHT4x_MEASUREMENT_LONG_MEDIUM_HEAT  0x2F
#define SHT4x_MEASUREMENT_SHORT_MEDIUM_HEAT 0x24
#define SHT4x_MEASUREMENT_LONG_LOW_HEAT     0x1E
#define SHT4x_MEASUREMENT_SHORT_LOW_HEAT    0x15

//  error codes
#define SHT4x_OK                            0x00
#define SHT4x_ERR_WRITECMD                  0x81
#define SHT4x_ERR_READBYTES                 0x82
#define SHT4x_ERR_HEATER_OFF                0x83
#define SHT4x_ERR_NOT_CONNECT               0x84
#define SHT4x_ERR_CRC_TEMP                  0x85
#define SHT4x_ERR_CRC_HUM                   0x86
#define SHT4x_ERR_CRC_STATUS                0x87
#define SHT4x_ERR_HEATER_COOLDOWN           0x88
#define SHT4x_ERR_HEATER_ON                 0x89
#define SHT4x_ERR_SERIAL_NUMBER_CRC         0x8A


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
  uint32_t lastRead();

  bool reset(bool fast = false);

  //  getHumidity returns 0..100%
  float    getHumidity();
  //  getTemperature returns degrees Celsius
  float    getTemperature();
  float    getFahrenheit();
  //  raw data e.g. debugging or efficient logging / transmit.
  uint16_t getRawHumidity();
  uint16_t getRawTemperature();


  //  ASYNC INTERFACE
  bool requestData(uint8_t measurementType = SHT4x_MEASUREMENT_SLOW);
  bool dataReady();
  bool readData(bool errorCheck = true);

  //  MISC
  int getError();  //  clears error flag
  //  fast == true, => skips CRC check
  bool getSerialNumber(uint32_t &serial, bool errorCheck = true);

  //  Heat protection
  bool heatingReady();
  void setHeatProtection(bool activateHeatProtection);


protected:
  uint8_t  _address;
  uint16_t _delay;
  uint32_t _lastRead;
  uint32_t _lastRequest;      // for async interface
  uint16_t _heatInterval;     // for overheating protection
  uint32_t _lastHeatRequest;  // for overheating protection
  uint16_t _rawHumidity;
  uint16_t _rawTemperature;
  uint8_t  _error;
  bool     _heatProtection;

  uint16_t getDelay(uint8_t measurementType);
  bool     validateMeasCmd(uint8_t cmd);
  bool     isHeatCmd(uint8_t cmd);                    // for overheating protection
  void     setHeatInterval(uint8_t measurementType);  // for overheating protection
  uint8_t  crc8(const uint8_t *data, uint8_t len);
  virtual bool writeCmd(uint8_t cmd);
  virtual bool readBytes(uint8_t n, uint8_t *val);
  TwoWire* _wire;
};


//  -- END OF FILE --