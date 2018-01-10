#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include "Sensor.h"
#include "connection.h"


/**
* Enumeration for different NXT light sensor LED modes
* @see Light
*/
enum Led_mode{
  LED_ON = 1,
  LED_OFF =2
};

using namespace std;
/**
* Class for NXT light sensor
*/
class Light : public Sensor{
  public:
    /**
     * Constructor for the light sensor (percent mode and LED on is default)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param mode [set the sensor mode (default is percent mode)]
     * @param led [set the LED mode to either LED_ON or LED_OFF (default is LED_ON)]
    */
    Light(Sensor_port port, Connection* connection, Led_mode led=LED_ON):Sensor(port, connection, LIGHT_ACTIVE, PERCENT_MODE){
      if(led == LED_OFF){
        sensor_type = LIGHT_INACTIVE;
      }
    }

    ~Light(){}

    /**
     * Turn the LED on or off
     * @param value [use LED_ON or LED_OFF to determine the state of the LED]
    */
    void set(unsigned int value){
       if(value == LED_ON){
         sensor_type = LIGHT_ACTIVE;
       }
       if(value == LED_OFF){
         sensor_type = LIGHT_INACTIVE;
       }
       if(this->has_init){
          this->init(true);
       }
       return;
    }

};
#endif
