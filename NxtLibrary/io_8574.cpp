#include "io_8574.h"
#include <sstream>
#include <string>
Io_8574::Io_8574(Sensor_port port, Connection *connection, unsigned char i2c_address):I2c(port,connection, LOWSPEED, i2c_address){

}

Io_8574::~Io_8574(){}


void Io_8574::init(bool reply){
  I2c::init(reply);
  read();
  read();
  this->has_init=true;
}

void Io_8574::set(unsigned int value){
  if(!this->has_init){
    init();
  }
  unsigned char command[2];
  command[0] = i2c_address;
  command[1] = value;
  i2c_write(&command[0], 2, 0);
  wait_for_bytes(0);//wait for byte to be ready
}

int Io_8574::read(){
  if(!this->has_init){
    init();
  }
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[1];
  command[0]=i2c_address;
  i2c_write(&command[0], 1, 1);
  wait_for_bytes(1);
  i2c_read(&rx_buffer[0], 1);
  return  rx_buffer[1] & 0xff; //returns unsigned value
}

Sensor_type Io_8574::get_type(){
  return IO_8574_SENSOR;
}

std::string Io_8574::print(){
  std::stringstream out;
  out << read();
  return out.str();
}


