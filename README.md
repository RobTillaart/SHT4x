
[![Arduino CI](https://github.com/RobTillaart/SHT4x/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/SHT4x/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/SHT4x/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/SHT4x/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/SHT4x/actions/workflows/jsoncheck.yml)
[![GitHub issues](https://img.shields.io/github/issues/RobTillaart/SHT4x.svg)](https://github.com/RobTillaart/SHT4x/issues)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/SHT4x/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/SHT4x.svg?maxAge=3600)](https://github.com/RobTillaart/SHT4x/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/robtillaart/library/SHT4x.svg)](https://registry.platformio.org/libraries/robtillaart/SHT4x)


# SHT4x

Arduino library for the SHT4x temperature and humidity sensor.


## Description

The SHT4x family of sensors should work up to 1 MHz I2C.

Accuracy table

|  Sensor  |  Temperature   |  Humidity  |  Verified 
|:--------:|:--------------:|:----------:|:----------:|
|   SHT40  |      ~0.2 °C    |     1.8%   |     Y      |
|   SHT41  |      ~0.2 °C    |     1.8%   |     N      |
|   SHT43  |      ISO Cal   |     1.8%   |     N      |
|   SHT45  |      ~0.1 °C    |     1.0%   |     N      |


An elaborated library for the SHT4x sensor can be found here
- https://github.com/Sensirion/arduino-i2c-sht4x


### I2C multiplexing

Sometimes you need to control more devices than possible with the default
address range the device provides.
This is possible with an I2C multiplexer e.g. TCA9548 which creates up
to eight channels (think of it as I2C subnets) which can use the complete
address range of the device.

Drawback of using a multiplexer is that it takes more administration in
your code e.g. which device is on which channel.
This will slow down the access, which must be taken into account when
deciding which devices are on which channel.
Also note that switching between channels will slow down other devices
too if they are behind the multiplexer.

- https://github.com/RobTillaart/TCA9548


### Related

SHT series temperature sensors

- https://github.com/RobTillaart/SHT2x
- https://github.com/RobTillaart/SHT31 Sensirion humidity / temperature sensor
- https://github.com/RobTillaart/SHT31_SW  = softWire based I2C.
- https://github.com/RobTillaart/SHT31_SWW = softWire based I2C.
- https://github.com/RobTillaart/SHT85 Sensirion humidity / temperature sensor
- https://github.com/RobTillaart/tinySHT2x

Other temperature sensors

- https://github.com/RobTillaart/DHTNew DHT11/22 etc
- https://github.com/RobTillaart/DHTStable DHT11/22 etc
- https://github.com/RobTillaart/DHT_Simulator
- https://github.com/RobTillaart/DS18B20_INT OneWire temperature sensor
- https://github.com/RobTillaart/DS18B20_RT OneWire temperature sensor

An elaborated library for the SHT31 sensor can be found here
- https://github.com/hawesg/SHT31D_Particle_Photon_ClosedCube

Other, including Dewpoint, heatindex, related functions and conversions.

- https://www.kandrsmith.org/RJS/Misc/Hygrometers/calib_many.html (interesting)
- https://github.com/RobTillaart/Temperature (conversions, dewPoint, heat index etc.)


## Interface

```cpp
#include "SHT4x.h"
```

### Constructor

- **SHT4x(uint8_t address = SHT_DEFAULT_ADDRESS, TwoWire \*wire = &Wire)** constructor. 
Optional select address and the I2C bus (Wire, Wire1 etc).
- **bool begin()** 
Returns false if device address is incorrect or device cannot be reset.
- **bool isConnected()** check sensor is reachable over I2C. Returns false if not connected.
- **uint8_t getAddress()** returns address set in the constructor.


### Read

- **bool read(uint8_t measurementType = SHT4x_MEASUREMENT_SLOW, bool errorCheck = true)** 
  - **SHT4x_MEASUREMENT_SLOW** : High precision measurement
  - **SHT4x_MEASUREMENT_MEDIUM** : Medium precision measurement
  - **SHT4x_MEASUREMENT_FAST** : Low precision measurement
  - **SHT4x_MEASUREMENT_LONG_HIGH_HEAT** : activate heater with 200mW for 1s including a high precision measurement just before deactivation
  - **SHT4x_MEASUREMENT_SHORT_HIGH_HEAT** : activate heater with 200mW for 0.1s including a high precision measurement just before deactivation
  - **SHT4x_MEASUREMENT_LONG_MEDIUM_HEAT** :activate heater with 110mW for 1s including a high precision measurement just before deactivation
  - **SHT4x_MEASUREMENT_SHORT_MEDIUM_HEAT** :activate heater with 110mW for 0.1s including a high precision measurement just before deactivation
  - **SHT4x_MEASUREMENT_LONG_LOW_HEAT** :activate heater with 20mW for 1s including a high precision measurement just before deactivation
  - **SHT4x_MEASUREMENT_SHORT_LOW_HEAT** :activate heater with 20mW for 0.1s including a high precision measurement just before deactivation
  - **errorCheck**: errorCheck = true does the CRC check. Returns false if reading fails or in case of a CRC failure. Equivalent to fast = false in the SHT31 library. 

Meta information about the sensor.

- **uint32_t lastRead()** in milliSeconds since start of program.
- **bool reset()** resets the sensor, soft reset by default. Returns false if call fails.

The following functions will return the same value until a new **read()** call (or async) is made.

- **float getHumidity()** computes the relative humidity in % based on the latest raw reading, and returns it.
- **float getTemperature()** computes the temperature in °C based on the latest raw reading, and returns it.
- **float getFahrenheit()** computes the temperature in °F based on the latest raw reading, and returns it..


The **getRawHumidity()** and **getRawTemperature()** can be used to minimize storage or communication as the data type is 50% smaller.
Another application is faster comparison with a previous value or threshold.
However comparisons are quite fast.

- **uint16_t getRawHumidity()** returns the raw two-byte representation of humidity directly from the sensor.
- **uint16_t getRawTemperature()** returns the raw two-byte representation of temperature directly from the sensor.

Note that the temperature and humidity values are recalculated on every call to getHumidity() and getTemperature(). 
If you're worried about the extra cycles, you should make sure to cache these values or only request them after 
you've performed a new reading.


### Error interface

- **int getError()** returns last set error flag and clear it. 
Be sure to clear the error flag by calling **getError()** before calling 
any command as the error flag could be from a previous command.

|  Error  |  Symbolic                     |  Description                   |
|:-------:|:------------------------------|:-------------------------------|
|  0x00   |  SHT4x_OK                     |  no error                      |
|  0x81   |  SHT4x_ERR_WRITECMD           |  I2C write failed              |
|  0x82   |  SHT4x_ERR_READBYTES          |  I2C read failed               |
|  0x83   |  SHT4x_ERR_HEATER_OFF         |  Could not switch off heater   |
|  0x84   |  SHT4x_ERR_NOT_CONNECT        |  Could not connect             |
|  0x85   |  SHT4x_ERR_CRC_TEMP           |  CRC error in temperature      |
|  0x86   |  SHT4x_ERR_CRC_HUM            |  CRC error in humidity         |
|  0x87   |  SHT4x_ERR_CRC_STATUS         |  CRC error in status field     |
|  0x88   |  SHT4x_ERR_HEATER_COOLDOWN    |  Heater need to cool down      |
|  0x89   |  SHT4x_ERR_HEATER_ON          |  Could not switch on heater    |
|  0x8A   |  SHT4x_ERR_SERIAL_NUMBER_CRC  |  Could not switch on heater    |

### Async interface

See async example for usage

- **bool requestData(uint8_t measurementType = SHT4x_MEASUREMENT_SLOW)** requests a new measurement. Returns false if this fails. See read() for the possible input.
- **bool dataReady()** checks if enough time has passed to read the data.
- **bool readData(bool errorCheck = true)** errorCheck = true does the CRC check. Returns false if reading fails or in case of a CRC failure. Equivalent to fast = false in the SHT31 library. 


### GetSerial

- **bool getSerialNumber(uint32_t &serial, bool errorCheck = true)** errorCheck == true, => CRC check, slower
errorCheck == false, => no CRC check, faster. 


## Future

#### Must

- Validate and compile

#### Should

- Validate the I2C speed


#### Could

- move code from .h to .cpp


#### Wont


## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,


