#ifndef TOUCH_SENSOR_H
#define TOUCH_SENSOR_H
#include "Sensor.h"
#include "connection.h"

using namespace std;
/**
* Class for NXT and RCX touch sensor
*/
class Touch : public Sensor{
  public:
    /**
     * Constructor for the touch sensor (default mode is bool)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
    */
    Touch(Sensor_port port, Connection* connection):Sensor(port, connection, TOUCH, BOOL_MODE){}
    ~Touch(){}

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return either "Pressed" or "Released"
    */
    string print(){
      if(read()){
        return "Pressed";
      }
      else{
        return "Released";
      }
    }

};
#endif

