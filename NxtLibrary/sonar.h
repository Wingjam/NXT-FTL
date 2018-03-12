#ifndef SONAR_CLASS
#define SONAR_CLASS
#include "i2c.h"
#include "windows.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

//Modes
#define OFF 0x00
#define SINGLE_SHOT 0x01
#define CONTINUOUS_MEASUREMENT 0x02
#define EVENT_CAPTURE 0x03
#define REQUEST_WARM_RESET 0x04

//Sonar registers
#define SONAR_ADDRESS 0x02
#define INTERVAL 0x40
#define COMMAND 0x41
#define RESULT_1 0x42
#define RESULT_2 0x43
#define RESULT_3 0x44
#define RESULT_4 0x45
#define RESULT_5 0x46
#define RESULT_6 0x47
#define RESULT_7 0x48
#define RESULT_8 0x49
#define ZERO_VALUE 0x50
#define SCALE_FACTOR 0x51
#define SCALE_DIVISOR 0x52
#define FACTORY_ZERO_VALUE 0x11
#define FACTORY_SCALE_FACTOR 0x12
#define FACTORY_SCALE_DIVISOR 0x13


/**
* Enumeration for different sonar sensor modes
* @see Sonar
*/
enum NXTLIBRARY_API Sonar_mode {
  /**
  * Sensor reading is returned in centimeters
  */
  METRIC = 1,

  /**
  * Sensor reading is returned in inches
  */
  INCH = 2
};

using namespace std;

/**
* Class for the NXT sonar (ultrasonic) sensor
*/
class NXTLIBRARY_API Sonar: public I2c{
  public:
    /**
     * Constructor for the sonar sensor (percent mode and LED on is default)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param mode [set the sonar mode to either METRIC or INCH (default is METRIC)]
    */
    Sonar(Sensor_port port, Connection* connection, Sonar_mode mode=METRIC);
    ~Sonar();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void init(bool reply=false);

    /**
    * Read the sensor value
    * (init method will be called if the sensor has not been initialized)
    * @return the sensor reading in either inches or centimeters
    */
    int read();

    /**
     * Set the return value to be either inch or centimeter
     * @param value [use either METRIC or INCH to indicate the return value of read()]
     * @see Sonar_mode
    */
    void set(unsigned int value);

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string + either " CM" or " inch(s)"
    */
    string print();

    /**
     * Get the sensor type
     * @return either SONAR_METRIC or SONAR_INCH
    */
    Sensor_type get_type();

  protected:
    Sonar_mode sonar_mode;
    void off();
    void write_register(int reg_adr, int value);
    int read_register(int reg_adr);
    void set_mode(int mode);
    void sonar_reset();
    //void set_sensor_mode(int mode);
};


#endif
