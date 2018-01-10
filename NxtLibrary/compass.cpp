#include "compass.h"
#include "error.h"
#include <windows.h>

Compass::Compass(Sensor_port port, Connection* connection, unsigned int wait_time, unsigned char i2c_address):I2c(port, connection, LOWSPEED, i2c_address, wait_time){}

Compass::~Compass(){}

void Compass::init(bool reply){
  I2c::init(reply);
  Sleep(500);
  read();
  read();
  this->has_init=true;
}

int Compass::read(){
  if(!this->has_init){
    init();
  }
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]= COMPASS_ADDRESS;
  command[1]= DEGREE;
  i2c_write(&command[0], 2, 2);
  wait_for_bytes(2);
  i2c_read(&rx_buffer[0], 2);
  return (0xff & rx_buffer[1])*2 + rx_buffer[2];
}

Sensor_type Compass::get_type(){
  return COMPASS;
}

std::string Compass::print(){
  std::stringstream out;
  out << read() << " degrees";
  return out.str();
}
