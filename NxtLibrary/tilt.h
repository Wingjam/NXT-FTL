#ifndef  TILT_SENSOR
#define  TILT_SENSOR
#include <iostream>
#include <string>
#include "connection.h"
#include "I2C.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

//registers
#define TILT_ADDRESS 0x02
#define X_UPPER 0x42
#define Y_UPPER 0x43
#define Z_UPPER 0x44
#define X_LOWER 0x45
#define Y_LOWER 0x46
#define Z_LOWER 0x47

using namespace std;

/**
* Class to retrieve XYZ coordinates from the HiTechnic Tilt/Accekeration sensor
* @see Tilt#read
*/
NXTLIBRARY_API class Xyz_position : public Result{
  public:
    NXTLIBRARY_API Xyz_position(){}
    /**
    * Holds the x-coordinate
    */
    int x;

    /**
    * Holds the y-coordinate
    */
    int y;

    /**
    * Holds the z-coordinate
    */
    int z;

    /**
    * @return XYZ_RESULT
    */
    NXTLIBRARY_API Result_type get_type(){return XYZ_RESULT;}
};

/**
* Class for HiTechnic tilt/accekeration sensor
*/
NXTLIBRARY_API class Tilt: public I2c{
  public:
    /**
     * Constructor for the Tilt sensor
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param i2c_address [set the I2C address of the sensor (just use the default address unless you have done a hack)]
    */
    NXTLIBRARY_API Tilt(Sensor_port port, Connection *connection, unsigned char i2c_address = TILT_ADDRESS);
    NXTLIBRARY_API ~Tilt();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    NXTLIBRARY_API void init(bool reply=false);

    /**
    * Get the X-Y-Z coordinates
    * (init method will be called if the sensor has not been initialized)
    * @param &result [must be of type Xyz_position - coordinates are "stored" in result]
    * @see Xyz_position
    */
    NXTLIBRARY_API int read(Result &result);

    /**
     * Get the sensor type
     * @return TILT
    */
    NXTLIBRARY_API Sensor_type get_type();

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string - "x='some value' y='some value' z='some value'"
    */
    NXTLIBRARY_API string print();
};
#endif
