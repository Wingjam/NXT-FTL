#pragma once
#include "Sensor.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;
/**
* Class for NXT and RCX touch sensor
*/
class NXTLIBRARY_API Touch : public Sensor{
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

