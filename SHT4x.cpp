//
//    FILE: SHT4x.cpp
//  AUTHOR: Samuel Cuerrier Auclair
//  AUTHOR: Rob Tillaart
// VERSION: 0.0.1
//    DATE: 2025-08-11
// PURPOSE: Arduino library for the SHT4x temperature and humidity sensor. High precision sensor with I2C interface.
//          This is a fork of the SHT31 library by Rob Tillaart, modified to work with the SHT4x series.
//          https://www.adafruit.com/product/2857
//     URL: https://github.com/RobTillaart/SHT4x


#include "SHT4x.h"


//  SUPPORTED COMMANDS
static constexpr uint16_t SHT4x_SOFT_RESET        = 0x94;

static constexpr uint16_t SHT4x_GET_SERIAL_NUMBER = 0x89;


SHT4x::SHT4x(uint8_t address, TwoWire *wire)
{
  _address        = address;
  _wire           = wire;
  _lastRead       = 0;
  _rawTemperature = 0;
  _rawHumidity    = 0;
  _error          = SHT4x_OK;
}


bool SHT4x::begin()
{
  if ((_address != 0x44) && (_address != 0x45) && (_address != 0x46))
  {
    _error = SHT4x_ERR_WRITECMD;
    return false;   //  invalid address
  }
  return reset();
}


bool SHT4x::isConnected()
{
  _wire->beginTransmission(_address);
  int rv = _wire->endTransmission();
  if (rv != 0) _error = SHT4x_ERR_NOT_CONNECT;
  return (rv == 0);
}


uint8_t SHT4x::getAddress()
{
  return _address;
}


bool SHT4x::read(uint8_t measurementType, bool errorCheck)
{
  if (!requestData(measurementType))
  {
    return false;
  }
  delay(getDelay()); 
  return readData(errorCheck);
}

bool SHT4x::reset()
{
  bool b = writeCmd(SHT4x_SOFT_RESET);
  if (b == false)
  {
    return false;
  }
  delay(1);   //  table 5 datasheet
  return true;
}

/////////////////////////////////////////////////////////////////
//
//  ASYNCHRONUOUS INTERFACE
//
bool SHT4x::requestData(uint8_t measurementType)
{
  if (!validateMeasCmd(measurementType))
  {
    _error = SHT4x_ERR_WRITECMD;
    return false;
  }
  if (writeCmd(measurementType) == false)
  {
    return false;
  }
  _lastRequest = millis();
  _lastMeasurementType = measurementType;
  return true;
}


bool SHT4x::dataReady()
{
  return ((millis() - _lastRequest) > getDelay());
}


bool SHT4x::readData(bool errorCheck)
{
  uint8_t buffer[6];
  if (readBytes(6, (uint8_t*) &buffer[0]) == false)
  {
    return false;
  }

  if (errorCheck)
  {
    if (buffer[2] != crc8(buffer, 2))
    {
      _error = SHT4x_ERR_CRC_TEMP;
      return false;
    }
    if (buffer[5] != crc8(buffer + 3, 2))
    {
      _error = SHT4x_ERR_CRC_HUM;
      return false;
    }
  }

  _rawTemperature = (buffer[0] << 8) + buffer[1];
  _rawHumidity    = (buffer[3] << 8) + buffer[4];

  _lastRead = millis();
  return true;
}


/////////////////////////////////////////////////////////////////
//
//  MISC
//
int SHT4x::getError()
{
  int rv = _error;
  _error = SHT4x_OK;
  return rv;
}


/**
 * See https://sensirion.com/media/documents/E5762713/63D103C2/Sensirion_electronic_identification_code_SHT3x.pdf
 */
bool SHT4x::getSerialNumber(uint32_t &serial, bool errorCheck) {
  if (writeCmd(SHT4x_GET_SERIAL_NUMBER) == false) {
      return false;
  }
  delay(1);
  uint8_t buffer[6];
  if (readBytes(6, &buffer[0]) == false) {
    return false;
  }

  if (errorCheck) {
      if (buffer[2] != crc8(buffer, 2)) {
      _error = SHT4x_ERR_SERIAL_NUMBER_CRC;
      return false;
      }
      if (buffer[5] != crc8(buffer + 3, 2)) {
      _error = SHT4x_ERR_SERIAL_NUMBER_CRC;
      return false;
      }
  }
  serial = buffer[0];
  serial <<= 8;
  serial += buffer[1];
  serial <<= 8;
  serial += buffer[3];
  serial <<= 8;
  serial += buffer[4];
  return true;
}


/////////////////////////////////////////////////////////////////
//
//  PROTECTED
//
uint32_t SHT4x::getDelay()
{
  //  table 5 datasheet
  switch(_lastMeasurementType)
  {
    case SHT4x_MEASUREMENT_SLOW:
      return 9;
    case SHT4x_MEASUREMENT_MEDIUM:
      return 5;
    case SHT4x_MEASUREMENT_FAST:
      return 2;
    case SHT4x_MEASUREMENT_LONG_HIGH_HEAT:
    case SHT4x_MEASUREMENT_LONG_MEDIUM_HEAT:
    case SHT4x_MEASUREMENT_LONG_LOW_HEAT:
      return 1100;
    case SHT4x_MEASUREMENT_SHORT_HIGH_HEAT:
    case SHT4x_MEASUREMENT_SHORT_MEDIUM_HEAT:
    case SHT4x_MEASUREMENT_SHORT_LOW_HEAT:
      return 110;
  }
  return 0;   //  Happen if dataReady get called before requestData
}

uint8_t SHT4x::crc8(const uint8_t *data, uint8_t len)
{
  //  CRC-8 formula from page 14 of SHT spec pdf
  const uint8_t POLY(0x31);
  uint8_t crc(0xFF);

  for (uint8_t j = len; j; --j)
  {
    crc ^= *data++;

    for (uint8_t i = 8; i; --i)
    {
      crc = (crc & 0x80) ? (crc << 1) ^ POLY : (crc << 1);
    }
  }
  return crc;
}


bool SHT4x::writeCmd(uint8_t cmd)
{
  _wire->beginTransmission(_address);
  _wire->write(cmd);
  if (_wire->endTransmission() != 0)
  {
    _error = SHT4x_ERR_WRITECMD;
    return false;
  }
  _error = SHT4x_OK;
  return true;
}


bool SHT4x::readBytes(uint8_t n, uint8_t *val)
{
  int rv = _wire->requestFrom(_address, (uint8_t) n);
  if (rv == n)
  {
    for (uint8_t i = 0; i < n; i++)
    {
      val[i] = _wire->read();
    }
    _error = SHT4x_OK;
    return true;
  }
  _error = SHT4x_ERR_READBYTES;
  return false;
}


//  -- END OF FILE --