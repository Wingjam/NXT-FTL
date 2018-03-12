#ifndef  IO_PCF8591
#define  IO_PCF8591
#include <iostream>
#include <string>
#include "I2C.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;

/**
* Class for the PCF8574 8-bit I/O I2C chip
*/
NXTLIBRARY_API class Io_8574 : public I2c{
  public:
    /**
     * Constructor for the PCF8574 I/O chip
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param i2c_address [set the I2C address of the chip]
    */
    NXTLIBRARY_API Io_8574(Sensor_port port, Connection *connection, unsigned char i2c_address);
    NXTLIBRARY_API ~Io_8574();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    NXTLIBRARY_API void init(bool reply = false);

    /**
    * Read the sensor value
    * (init method will be called if the sensor has not been initialized)
    * @return a value between 0 and 255 that describes the state of the pins
    * (remember to set input pins to high before reading)
    */
    NXTLIBRARY_API int read();

    /**
     * Sets the pins high (write to the sensor)
     * @param value [sets the specified pins high (0xff all pins are set to input)]
    */
    NXTLIBRARY_API void set(unsigned int value);

    /**
     * Get the sensor type
     * @return IO_8574_SENSOR
    */
    NXTLIBRARY_API Sensor_type get_type();


    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string
    */
    NXTLIBRARY_API string print();
};
#endif

