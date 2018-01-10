#include "tilt.h"
#include <windows.h>
#include <sstream>
#include <string>

Tilt::Tilt(Sensor_port port, Connection *connection, unsigned char i2c_address):I2c(port,connection, LOWSPEED_9V, i2c_address){
}

Tilt::~Tilt(){}

void Tilt::init(bool reply){
  Xyz_position pos;
  I2c::init(reply);
  read(pos);
  read(pos);
  this->has_init=true;
}

int Tilt::read(Result &result){
  if(! (result.get_type() == XYZ_RESULT)){
    return 0;
  }
  if(!this->has_init){
    init();
  }
  Result *temp = (Result *) &result;
  Xyz_position *position = (Xyz_position *) temp;
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]= TILT_ADDRESS;
  command[1]= X_UPPER;
  i2c_write(&command[0], 2, 6);
  wait_for_bytes(6);
  i2c_read(&rx_buffer[0], 6);

  position->x=rx_buffer[1];                //load x axis upper 8 bits
  position->y=rx_buffer[2];                //load Y axis upper 8 bits
  position->z=rx_buffer[3];                //load z axis upper 8 bits

  if(position->x > 127) position->x -=256;//convert x to 10 bit value
    position->x=position->x*4 + rx_buffer[4];
  if (position->y > 127) position->y-=256;//convert y to 10 bit value
    position->y=position->y*4 + rx_buffer[5];
  if (position->z > 127) position->z-=256;//convert z to 10 bit value
    position->z=position->z*4 + rx_buffer[6];
  return 1;
}

Sensor_type Tilt::get_type(){
  return TILT;
}

std::string Tilt::print(){
   Xyz_position position;
   read(position);
   std::stringstream out;
   out << "x=" << position.x << " y=" << position.y <<" z=" << position.z;
   return out.str();
}

