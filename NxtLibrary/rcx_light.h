#ifndef RCX_LIGHT_SENSOR_H
#define RCX_LIGHT_SENSOR_H
#include "Sensor.h"
#include "connection.h"

using namespace std;
/**
* Class for RCX light sensor
*/
class Light_rcx : public Sensor{
  public:
    /**
     * Constructor for the RCX light sensor (percent mode is default)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
    */
    Light_rcx(Sensor_port port, Connection* connection):
    Sensor(port, connection, REFLECTION, PERCENT_MODE){}

    //deconstructor
    ~Light_rcx(){}
};
#endif



