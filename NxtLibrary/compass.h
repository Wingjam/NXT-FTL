#ifndef COMPASS_CLASS
#define COMPASS_CLASS

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

//couldn't find any documentation for the HiTechnic NXT Compass Sensor so I used
// http://nxtasy.org/2006/09/02/a-hitechnic-compass-robotc-and-100-lines-of-code

#include "i2c.h"
#include "connection.h"

#define COMPASS_ADDRESS 0x02
//registers
#define COMMAND_REG 0x41
#define DEGREE 0x42
#define DEGREE_HALF 0x43

using namespace std;

/**
* Class for HiTechnic compass sensor
*/
class NXTLIBRARY_API Compass : public I2c{
  public:

    /**
     * Constructor for the compass sensor
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param wait_time [the time in ms between checking for reply]
     *(use default wait time otherwise COMMUNICATION_BUS_ERROR will frequently be thrown if sensor is pulled continuously)
     * @param i2c_address [set the I2C address of the sensor (just use the default address unless you have done a hack)]
    */
    Compass(Sensor_port port, Connection* connection, unsigned int wait_time=40, unsigned char i2c_address = COMPASS_ADDRESS);
    ~Compass();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void init(bool reply=false);

    /**
    * Read the sensor value
    * (init method will be called if the sensor has not been initialized)
    * @return degree (0-360) 0= North
    */
    int read();

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string + " degrees"
    */
    string print();

    /**
     * Get the sensor type
     * @return COMPASS
    */
    Sensor_type get_type();
  private:
};
#endif

