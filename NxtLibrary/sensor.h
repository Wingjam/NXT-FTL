#ifndef SENSOR
#define SENSOR
#include "connection.h"
#include <string.h>

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;
/**
  * Enumeration used to determine the result type (which sub-class of Result)
  * @see Result#get_type
 */
enum NXTLIBRARY_API Result_type{
  /**
  * Means that the sub class is Rgb_color (used with the color sensor)
  * @see Rgb_color
  * @see Color_sensor#read
  */
  RGB_RESULT=0,

   /**
  * Means that the sub class is Xyz_position (used with the tilt sensor)
  * @see Xyz_position
  * @see Tilt#read
  */
  XYZ_RESULT=1,

  /**
  * Means that the sub class is ADC_ports (used with the PCF8591 ADC chip/sensor)
  * @see Adc_ports
  * @see Adc_8591#read
  */
  ADC_RESULT=2
};

/**
  * Class used to return sensor readings when there is more than one return value
  * @see Sensor#read
 */
class NXTLIBRARY_API Result{
  public:
  /**
  * @return the result type (see the RESULT_TYPE enumeration)
  */
  virtual Result_type get_type()=0;
};

/**
  * Sensor port enumeration
 */
enum NXTLIBRARY_API Sensor_port {
  /**
   * Port 1
   */
  IN_1 = 0,

  /**
   * Port 2
   */
  IN_2 = 1,

  /**
   * Port 3
   */
  IN_3 = 2,

  /**
   * Port 4
   */
  IN_4 = 3
};

/**
* Sensor type enumeration
*/
enum NXTLIBRARY_API Sensor_type {
  /**
   * No sensor configured
   */
  NO_SENSOR = 0x00,

  /**
   * NXT or RCX touch sensor
   */
  TOUCH = 0x01,

  /**
   * RCX temperature sensor (maybe it workes with the NXT temperature sensor?)
   */
  TEMPERATURE = 0x02,

  /**
   * RCX light sensor
   */
  REFLECTION = 0x03,

  /**
   * RCX rotation sensor
   */
  ANGLE = 0x04,

  /**
   * NXT light sensor with LED on
   */
  LIGHT_ACTIVE = 0x05,

  /**
   * NXT light sensor with LED off
   */
  LIGHT_INACTIVE = 0x06,

  /**
   * NXT sound sensor with dB scaling
   */
  SOUND_DB = 0x07,

  /**
   * NXT sound sensor with dBA scaling
   */
  SOUND_DBA = 0x08,

  /**
   * Custom sensor type
   */
  CUSTOM = 0x09,

  /**
   * I2C digital sensor
   */
  LOWSPEED = 0x0A,

  /**
   * I2C digital sensor 9V
   */
  LOWSPEED_9V = 0x0B,

  /**
   * Sonar sensor returning metric distance - only used as a return type can not be used to set-up a sensor
   */
  SONAR_METRIC = 0x0C,

  /**
   * Sonar sensor returning inch distance - only used as a return type can not be used to set-up a sensor
   */
  SONAR_INCH = 0x0D,

  /**
   * HiTechnic compass sensor - only used as a return type can not be used to set-up a sensor
   */
  COMPASS = 0x0E,

  /**
   * PCF8574 I2C I/O chip - only used as a return type can not be used to set-up a sensor
   */
  IO_8574_SENSOR = 0x0F,

  /**
   * PCF8591 8-bit ADC chip - only used as a return type can not be used to set-up a sensor
   */
  ADC_8591_SENSOR = 0x10,

  /**
   * HiTechnic color sensor - only used as a return type can not be used to set-up a sensor
   */
  COLOR = 0x11,

  /**
   * HiTechnic gyro sensor - only used as a return type can not be used to set-up a sensor
   */
  GYRO = 0x12,

  /**
   * HiTechnic tilt sensor - only used as a return type can not be used to set-up a sensor
   */
  TILT = 0x13
};

/**
* Sensor mode enumeration
*/
enum NXTLIBRARY_API Sensor_mode {
  /**
   * Raw value from 0 to 1023
   */
  RAW_MODE = 0x00,

  /**
   * Boolean value
   */
  BOOL_MODE = 0x20,

  /**
   * Counts number of boolean transitions
   */
  TRANSITION_MODE = 0x40,

  /**
   * Counts number of boolean periods
   */
  PERIOD_MODE = 0x60,

  /**
   * Scales the raw value and returns a value from 0 to 100
   */
  PERCENT_MODE = 0x80,

  /**
   * Degrees in celsius
   */
  CELSIUS_MODE = 0xA0,

  /**
   * Degrees in Fahrenheit
   */
  FAHRENHEIT_MODE = 0xC0,

  /**
   * Rotation (16 ticks per revolution)
   */
  ANGLE_MODE = 0xE0,
};

/**
* Class for analog sensors - also works as the base class for all other sensors
*/
class NXTLIBRARY_API Sensor{
  public:
    /**
     * Sensor constructor
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param sensor_type [set the sensor type]
     * @param sensor mode [set the sensor mode]
    */
    Sensor(Sensor_port port, Connection* connection, Sensor_type sensor_type, Sensor_mode sensor_mode);

    virtual ~Sensor();

    /**
     * Initiate the sensor (sets its type and mode)
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    virtual void init(bool reply=false);

    /**
    * Get the sensor reading (mode dependent)
    * (init method will be called if the sensor has not been initialized)
    * @return the mode dependent sensor reading
    */
    virtual int read();

    /**
    * Get multiple sensor readings
    * (Dummy method does nothing - is implemented in some sub classes)
    * @param &result [reference where the sensor readings are placed]
    * @return 1 if sensor readings are placed in &result otherwise 0
    */
    virtual int read(Result &result);

    /**
    * Set a parameter on the sensor
    * (Dummy method does nothing - is implemented in some sub classes)
    */
    virtual void set(unsigned int value);

    /**
    * Get the sensor reading as a string
    * (init method will be called if the sensor has not been initialized)
    * @return the sensor reading as a string
    */
    virtual string print();

    /**
     * Get the sensor type
     * @return the sensor type
    */
    virtual Sensor_type get_type();

    /**
     * Get the sensor port
     * @return the sensor port
    */
    virtual Sensor_port get_port();

    /**
     * Get the sensor mode
     * @return the sensor mode
    */
    virtual Sensor_mode get_mode();

    /**
     * Get the I2C address
     * (Dummy method does nothing - is implemented in some sub class )
     * @return the I2C address
    */
    virtual unsigned char get_i2c_address();


  protected:
    /**
     * Indicates whether or not the sensors has been initiated
    */
    bool has_init;

    /**
     * Reset the sensor (sets its type and mode)
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void reset(bool reply=false);

    /**
     * Get the sensor values (updates the raw_AD_valued, normalized_AD_value, scaled_value, calibrated_value fields)
    */
    void get_sensor_value();

    /**
     * Get the mode dependent sensor reading
     * @return the updated scaled_value field
    */
    int read_mode();

    /**
     * Get the device dependent sensor reading
     * @return the updated raw_AD_value field
    */
    unsigned int read_raw();

    /**
     * Get the mode type dependent sensor reading
     * @return the updated normalized_AD_value field
    */
    unsigned int read_normalized();//type dependent

    /**
     * Set the type and mode of the sensor
    */
    void set_type_and_mode(Sensor_type type, Sensor_mode mode, bool reply=false);
    Connection *connection;
    Sensor_type sensor_type;
    Sensor_mode sensor_mode;
    Sensor_port sensor_port;
    unsigned int raw_AD_value;//device dependent
    unsigned int normalized_AD_value;//type dependent
    int scaled_value;//mode dependent
    int calibrated_value; //not used

};
#endif
