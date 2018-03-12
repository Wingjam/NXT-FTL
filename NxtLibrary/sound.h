#ifndef SOUND_SENSOR_H
#define SOUND_SENSOR_H
#include "Sensor.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif


/**
* Enumeration for different NXT sound sensor modes
* @see Sound
*/
NXTLIBRARY_API enum Sound_mode{
  /**
  * normal decibel scale
  */
  DB_MODE = 10,

  /**
  * decibel scale which is less sensitive to very high and very low frequencie (A scale)
  */
  DBA_MODE = 11
};

using namespace std;
/**
* Class for NXT sound sensor
*/
NXTLIBRARY_API class Sound : public Sensor{
  public:
    /**
     * Constructor for the sound sensor (percent mode and DBA_MODE is default)
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param sound_mode [set the sound mode to either DB_MODE or DBA_MODE (default is DBA_MODE)]
    */
    NXTLIBRARY_API Sound(Sensor_port port, Connection* connection, Sound_mode sound_mode=DBA_MODE):Sensor(port, connection, SOUND_DBA, PERCENT_MODE){
      if(sound_mode == DB_MODE){
        sensor_type = SOUND_DB;
      }
    }
    NXTLIBRARY_API ~Sound(){}

    /**
     * Get the sensor reading as a string
     * (init method will be called if the sensor has not been initialized)
     * @return sensor reading as a string + either " dBA" or " dB"
    */
    NXTLIBRARY_API string print(){
      if(sensor_type == SOUND_DBA){
        return Sensor::print() + " dBA";
      }
      else{
        return Sensor::print() + " dB";
      }
    }
};
#endif

