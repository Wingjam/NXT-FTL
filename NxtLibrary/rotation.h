#ifndef ROTATION_SENSOR_H
#define ROTATION_SENSOR_H
#include "Sensor.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;
#define RESET_TICKS 0
/**
* Class for RCX rotation sensor
*/
class NXTLIBRARY_API Rotation : public Sensor{
  public:
    /**
     * Constructor for the rotation sensor
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
    */
    Rotation(Sensor_port port, Connection* connection):Sensor(port, connection, ANGLE, ANGLE_MODE){}
    ~Rotation(){};

   /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return ticks + " ticks"
    */
    string print(){
      return Sensor::print() + " ticks";
    }

    /**
     * Reset the tick count
     * @param value [use RESET_TICKS to reset the tick count]
    */
    void set(unsigned int value){
      if(value == RESET_TICKS){
        this->reset();
      }
    }
};
#endif


