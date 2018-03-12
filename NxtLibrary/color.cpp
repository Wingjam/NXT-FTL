#include "stdafx.h"
#include "color.h"
#include <windows.h>
#include <sstream>
#include <string>

Color_sensor::Color_sensor(Sensor_port port, Connection *connection, unsigned int wait_time, unsigned char i2c_address):I2c(port,connection, LOWSPEED, i2c_address ,wait_time){}

Color_sensor::~Color_sensor(){}


int Color_sensor::read(Result &result){
  if(! (result.get_type() == RGB_RESULT)){
    return 0;
  }
  if(!this->has_init){
    init();
  }
  Result *temp;
  temp = (Result *) &result;
  Rgb_color *color = (Rgb_color*)(temp);
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]= COLOR_ADDRESS;
  command[1]= RED_RAW_LOW;
  i2c_write(&command[0], 2, 6);
  wait_for_bytes(6);
  i2c_read(&rx_buffer[0], 6);
  color->red = ((0xff & rx_buffer[2]) | ((0xff & rx_buffer[1]) << 8));
  color->green = ((0xff & rx_buffer[4]) | ((0xff & rx_buffer[3]) << 8));
  color->blue = ((0xff & rx_buffer[6]) | ((0xff & rx_buffer[5]) << 8));
  return 1;
}

void Color_sensor::init(bool reply){
  Rgb_color color;
  I2c::init(reply);
  read(color);
  read(color);
  this->has_init=true;
}

int Color_sensor::read(){
  if(!this->has_init){
    init();
  }
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]= COLOR_ADDRESS;
  command[1]= COLOR_NUMBER;

  //test this
  /*i2c_write(&command[0], 2, 14);
  wait_for_bytes(14);
  i2c_read(&rx_buffer[0], 14);*/

  i2c_write(&command[0], 2, 1);
  wait_for_bytes(1);
  i2c_read(&rx_buffer[0], 1);
  /*printf("Color Number: %d\n", rx_buffer[1]);
  printf("Red reading: %d\n", rx_buffer[2]);
  printf("Green reading: %d\n", rx_buffer[3]);
  printf("Blue reading: %d\n", rx_buffer[4]);
  printf("Raw red - High: %d \n", rx_buffer[5]);
  printf("Raw red - Low: %d\n", rx_buffer[6]);
  printf("Raw Green - High: %d\n", rx_buffer[7]);
  printf("Raw Green - Low: %d\n", rx_buffer[8]);
  printf("Raw Blue - High: %d\n", rx_buffer[9]);
  printf("Raw Blue - Low: %d\n", rx_buffer[10]);
  printf("Color index: %d\n", rx_buffer[11]);
  printf("Normalized red: %d\n", rx_buffer[12]);
  printf("Normalized green: %d\n", rx_buffer[13]);
  printf("Normalized blue: %d\n", rx_buffer[14]);*/
  //printf(rx_buffer)
  return 0xff & rx_buffer[1]; //returns unsigned value
}

Sensor_type Color_sensor::get_type(){
  return COLOR;
}

std::string Color_sensor::print(){
   Rgb_color color;
   read(color);
   std::stringstream out;
   out << "red=" << color.red << " green=" << color.green <<" blue=" << color.blue;
   return out.str();
}
