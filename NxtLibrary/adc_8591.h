#ifndef  ADC_PCF8591
#define  ADC_PCF8591
#include <iostream>
#include <string>
#include "connection.h"
#include "I2C.h"

/**
* Class to retrieve ADC port reading from the the PCF8591 ADC chip/sensor
* @see Adc_8591#read
*/
class Adc_ports : public Result{
  public:
    Adc_ports(){}
    /**
    * Holds 8 bit value from port 0
    */
    unsigned int port0;

    /**
    * Holds 8 bit value from port 1
    */
    unsigned int port1;

    /**
    * Holds 8 bit value from port 2
    */
    unsigned int port2;

    /**
    * Holds 8 bit value from port 3
    */
    unsigned int port3;
    Result_type get_type(){return ADC_RESULT;}
};

using namespace std;
/**
* Class for the PCF8591 8-bit A/D D/A converter (D/A feurures not implemented yet!)
*/
class Adc_8591: public I2c{
  public:
    /**
     * Constructor for the PCF8591 ADC chip
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param i2c_address [set the I2C address of the chip]
    */
    Adc_8591(Sensor_port port, Connection *connection, unsigned char i2c_address);
    ~Adc_8591();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void init(bool reply=false);

    /**
    * Get value from all four AD ports
    * @param &result [must be of type Adc_ports - values are "stored" in result]
    * @see Adc_ports
    */
    int read(Result &result);

    /**
     * Get the sensor type
     * @return ADC_8591_SENSOR
    */
    Sensor_type get_type();

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string - "P0='some value' P1='some value' P2='some value' P3='some value'"
    */
    string print();
  private:
    int get_port_reading(unsigned int port);
};
#endif

