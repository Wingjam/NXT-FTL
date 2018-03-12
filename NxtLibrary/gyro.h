#ifndef GYRO_SENSOR
#define GYRO_SENSOR
#include "Sensor.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;

/**
* Class for HiTechnic Gyro sensor
*/
NXTLIBRARY_API class Gyro : public Sensor{
  public:
    /**
     * Constructor for the Gyro sensor (default offset is set to zero)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param offset [set the offset]
    */
    NXTLIBRARY_API Gyro(Sensor_port port, Connection* connection, int offset=0): Sensor(port, connection, CUSTOM, RAW_MODE){
      this->offset=offset;
    }
    NXTLIBRARY_API ~Gyro(){};

    /**
     * Read the sensor value ()
     * (init method will be called if the sensor has not been initialized)
     * @return change in angle pr second (deg/sec)
    */
    NXTLIBRARY_API int read(){
       return Sensor::read_raw()-offset;
    }

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string + " deg/sec"
    */
    NXTLIBRARY_API string print(){
       return Sensor::print() + " deg/sec";
    }

    /**
     * Get the sensor type
     * @return GYRO
    */
    NXTLIBRARY_API Sensor_type get_type(){
      return GYRO;
    }
  protected:
    int offset;
};
#endif
