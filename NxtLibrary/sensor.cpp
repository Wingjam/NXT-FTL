#include "sensor.h"
#include "error.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <sstream>
#include <string>


using namespace std;
Sensor::Sensor(Sensor_port port, Connection* connection, Sensor_type sensor_type, Sensor_mode sensor_mode){
  this->sensor_port = port;
  this->sensor_type = sensor_type;
  this->sensor_mode = sensor_mode;
  this->connection = connection;
  this->has_init=false;
  this->raw_AD_value=0;
  this->normalized_AD_value=0;
  this->scaled_value=0;
  this->calibrated_value=0;
}

Sensor::~Sensor(){};

void Sensor::init(bool reply){
  set_type_and_mode(sensor_type,sensor_mode,reply);
  Sleep(100);
  reset(reply);
  Sleep(100);
  this->has_init=true;
}

/*
int Sensor::read(unsigned int return_value){
  if(return_value == RAW_AD_VALUE){
    return read_raw();
  }
  if(return_value == NORMALIZED_VALUE){
      return read_normalized();
  }

  if(return_value == MODE_VALUE){
      return read_mode();
  }
  return 0;
}
void Sensor::off(){
  SENSOR_TYPE temp = this->sensor_type;
  this->sensor_type = NO_SENSOR;
  set_type_and_mode(sensor_type,sensor_mode,false);
  this->has_init = false;
  this->sensor_type = temp;
}
*/

int Sensor::read(Result &result){
  return 0;
}

int Sensor::read(){
  return read_mode();
}

string Sensor::print(){
   std::stringstream out;
   out << read();
   return out.str();
}

void Sensor::set(unsigned int value){
  return;
}

Sensor_port Sensor::get_port(){
  return this->sensor_port;
}

Sensor_mode Sensor::get_mode(){
  return this->sensor_mode;
}


void Sensor::set_type_and_mode(Sensor_type type, Sensor_mode mode,bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[7];
  command[0]=0x05;  //command length
  command[1]=0x00;

  //start of message
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x05;
  command[4]=sensor_port;
  command[5]=sensor_type;//input port
  command[6]=sensor_mode;//set mode
  connection->send(&command[0],7);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("set_type_and_mode","Sensor", 0x00FF & answer[4]);
    }
  }
}

void Sensor::get_sensor_value(){
  if(!this->has_init){
      this->init(false);
  }
  unsigned char command[5];
  unsigned char answer[NXT_BUFFER_SIZE];
  command[0]=0x03;
  command[1]=0x00;

  command[2]=0x00;
  command[3]=0x07;
  command[4]=sensor_port;
  connection->send(&command[0],5);
  connection->receive(&answer[0],18);
  if(answer[4]){
     throw Nxt_exception("get_sensor_value","Sensor", 0x00FF & answer[4]);
  }
  raw_AD_value=((0xff & answer[10]) | (answer[11] << 8));
  normalized_AD_value=((0xff & answer[12]) | (answer[13] << 8));

  //printf("answer[14]=0x%x\n",answer[14]);
  //printf("answer[15]=0x%x\n",answer[15]);

  scaled_value= ((0xff & answer[14]) | (answer[15] << 8));
  calibrated_value=((0xff & answer[16]) | (answer[17] << 8));
}

//mode dependent
int Sensor::read_mode(){
  get_sensor_value();
  return scaled_value;
}

//device dependent
unsigned int Sensor::read_raw(){
  get_sensor_value();
  return raw_AD_value;
}

//type dependent
unsigned int Sensor::read_normalized(){
  get_sensor_value();
  return normalized_AD_value;;
}

//
Sensor_type Sensor::get_type(){
  return sensor_type;
}

void Sensor::reset(bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[5];
  command[0]=0x03;  //command length
  command[1]=0x00;

  //start of message
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x08;
  command[4]=sensor_port;
  connection->send(&command[0],5);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("reset","Sensor", 0x00FF & answer[4]);
    }
  }
}

unsigned char Sensor::get_i2c_address(){
  return 0;
}

