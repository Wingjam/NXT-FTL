#ifndef I2C_CLASS
#define I2C_CLASS
#define I2C_BUFFER_SIZE 24
#define TIMEOUT 500
#include "Sensor.h"
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;
/**
* Abstract class for digital I2C sensors
*/
class NXTLIBRARY_API I2c : public Sensor{
  public:
    /**
     * I2C constructor
     * @param port [which sensor port to use]
     * @param *connection [attach a connection]
     * @param sensor_type [set the sensor type (should be LOWSPEED or LOWSPEED_9V )]
     * @param i2c_address [the I2C address of the sensor]
     * @param wait_time [the time in ms between checking for reply (should normally be zero but some sensor will produce COMMUNICATION_BUS_ERROR if pulled continuously) ]
    */
    I2c(Sensor_port port, Connection* connection, Sensor_type sensor_type, unsigned char i2c_address, unsigned int wait_time=0);
    ~I2c();

    /**
     * Initiate the sensor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    virtual void init(bool reply=false);

    /**
     * Get the sensor type
     * (must be implemented in sub class)
     * @return the sensor type
    */
    virtual Sensor_type get_type()=0;

    /**
     * Get the I2C address
     * @return the I2C address
    */
    unsigned char get_i2c_address();
  protected:
    void wait_for_bytes(int num_bytes);
    unsigned int wait_time;
    void update_sensor_status();
    void i2c_write(unsigned char *tx_buf, int tx_length, int rx_length);
    void i2c_read(unsigned char* rx_buffer, int rx_length);
    int bytes_ready;
    unsigned char i2c_address;
};
#endif
