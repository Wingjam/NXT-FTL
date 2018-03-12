#ifndef  COLOR_SENSOR
#define  COLOR_SENSOR
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
#define COLOR_ADDRESS 0x02
#define COLOR_NUMBER 0x42
#define RED_READING 0x43
#define GREEN_READING 0x44
#define BLUE_READING 0x45
#define RED_RAW_LOW 0x46
#define RED_RAW_HIGH 0x47
#define GREEN_RAW_LOW 0x48
#define GREEN_RAW_HIGH 0x49
#define BLUE_RAW_LOW 0x4A
#define BLUE_RAW_HIGH 0x4B
#define COLOR_INDEX_NO 0x4c
#define RED_NORMALIZED 0x4d
#define GREEN_NORMALIZED 0x4e
#define BLUE_NORMALIZED 0x4f

/**
* Class to retrieve red green and blue colors from the HiTechnic color sensor
* @see Color_sensor#read
*/
NXTLIBRARY_API class Rgb_color: public  Result{
  public:
    NXTLIBRARY_API Rgb_color(){};
    /**
    * Holds the raw 10-bit red color
    */
    unsigned int red;

    /**
    * Holds the raw 10-bit green color
    */
    unsigned int green;

    /**
    * Holds the raw 10-bit blue color
    */
    unsigned int blue;

    /**
    * @return RGB_RESULT
    */
    NXTLIBRARY_API Result_type get_type(){return RGB_RESULT;}
};

using namespace std;

/**
* Class for HiTechnic color sensor
*/
NXTLIBRARY_API class Color_sensor: public I2c{
  public:
    /**
     * Constructor for the compass sensor
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param wait_time [the time in ms between checking for reply]
     * (use default wait time otherwise COMMUNICATION_BUS_ERROR will frequently be thrown if sensor is pulled continuously)
     * @param i2c_address [set the I2C address of the sensor (just use the default address unless you have done a hack)]
    */

    NXTLIBRARY_API Color_sensor(Sensor_port port, Connection *connection, unsigned int wait_time = 40, unsigned char i2c_address = COLOR_ADDRESS);
    NXTLIBRARY_API ~Color_sensor();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    NXTLIBRARY_API void init(bool reply=false);

    /**
    * Get the color number (0-17)
    * (init method will be called if the sensor has not been initialized)
    * @return the color number (0-17)
    * @see <a href="http://www.hitechnic.com/" TARGET="_blank">color number chart can be found on the HiTechnic website</a>
    */
    NXTLIBRARY_API int read();

    /**
    * Get the RGB color
    * (init method will be called if the sensor has not been initialized)
    * @param &result [must be of type Rgb_color - RGB colors are "stored" in result]
    * @see Rgb_color
    */
    NXTLIBRARY_API int read(Result &result);

    /**
     * Get the sensor type
     * @return COLOR
    */
    NXTLIBRARY_API Sensor_type get_type();

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string - "red='some value' green='some value' blue='some value'"
    */
    NXTLIBRARY_API string print();
};
#endif


