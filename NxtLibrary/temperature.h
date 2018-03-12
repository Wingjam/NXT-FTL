#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H
#include "Sensor.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

/**
* Enumeration for different temperarure sensor modes
* @see Temperature
*/
NXTLIBRARY_API enum Temp_mode{
  CELSIUS = 21,
  FAHRENHEIT = 22
};

using namespace std;
/**
* Class for RCX temperature sensor
*/
NXTLIBRARY_API class Temperature : public Sensor{
  public:
    /**
     * Constructor for the temperature sensor (default TEMP_MODE is celsius)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param  mode [set the TEMP mode to either CELSIUS or FAHRENHEIT (default is CELSIUS)]
    */
    NXTLIBRARY_API Temperature(Sensor_port port, Connection* connection, Temp_mode mode = CELSIUS):Sensor(port, connection, TEMPERATURE, CELSIUS_MODE){
      if(mode == FAHRENHEIT){
        sensor_mode = FAHRENHEIT_MODE;
      }
    }
    NXTLIBRARY_API ~Temperature(){};

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string + either " Celsius" or " Fahrenheit"
    */
    NXTLIBRARY_API string print(){
      if(sensor_mode == FAHRENHEIT_MODE){
        return Sensor::print() + " Fahrenheit";
      }
      else{
        return Sensor::print() + " Celsius";
      }
    }
};
#endif

