#include "stdafx.h"
#include "adc_8591.h"
#include <sstream>
#include <string>

Adc_8591::Adc_8591(Sensor_port port, Connection *connection, unsigned char adress):I2c(port, connection, LOWSPEED, i2c_address){
}

Adc_8591::~Adc_8591(){}

void Adc_8591::init(bool reply){
  I2c::init(reply);
  get_port_reading(0x00);
  get_port_reading(0x00);
  this->has_init = true;
}

//should be optimized to a single I2C poll
int Adc_8591::read(Result &result){
  if(! (result.get_type() == ADC_RESULT)){
    return 0;
  }
  if(!this->has_init){
    init();
  }
  Result *temp = (Result *) &result;
  Adc_ports *ports = (Adc_ports *) temp;
  ports->port0 = get_port_reading(0x00);
  ports->port1 = get_port_reading(0x01);
  ports->port2 = get_port_reading(0x02);
  ports->port3 = get_port_reading(0x03);
  return 1;
}

int Adc_8591::get_port_reading(unsigned int port){
  if(!this->has_init){
    init();
  }
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]=i2c_address;
  command[1]=port;
  i2c_write(&command[0], 2, 1);
  wait_for_bytes(1);
  i2c_read(&rx_buffer[0], 1);
  return  0xff & rx_buffer[1];
}

Sensor_type Adc_8591::get_type(){
  return ADC_8591_SENSOR;
}

std::string Adc_8591::print(){
  std::stringstream out;
  out << "P0=" << get_port_reading(0x00) << " P1=" + get_port_reading(0x01) << " P2=" + get_port_reading(0x02) << " P3=" + get_port_reading(0x03);
  return out.str();
}


