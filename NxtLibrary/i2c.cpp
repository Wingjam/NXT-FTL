#include "i2c.h"
#include "error.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <windows.h>

I2c::I2c(Sensor_port port, Connection* connection, Sensor_type sensor_type, unsigned char i2c_address, unsigned int wait_time):Sensor(port, connection, sensor_type, RAW_MODE){
  this->wait_time = wait_time;
  this->i2c_address = i2c_address;
}

I2c::~I2c(){}

void I2c::init(bool reply){
  Sensor::init(reply);
  //Sleep(200);
  //reset(reply);
}

/*int I2C::read(){
  return 0;
}*/

void I2c::i2c_write(unsigned char *tx_buf, int tx_length, int rx_length){
  int i=0;
  unsigned char command[24];
  while(i < tx_length){
    command[i+7]=tx_buf[i];
    i++;
    //printf("i=%d\n",i);
  }
  command[0]=i+5;
  command[1]=0x00;
  command[2]=0x80;
  command[3]=0x0F;
  command[4]=sensor_port;
  command[5]=i;
  command[6]=rx_length;
  connection->send(&command[0],i+7);
}

void I2c::i2c_read(unsigned char* rx_buffer, int rx_length){
  int i;
  unsigned char command[5];
  wait_for_bytes(rx_length);
  command[0]=0x03;
  command[1]=0x00;
  command[2]=0x00;
  command[3]=0x10;
  command[4]=sensor_port;
  connection->send(&command[0],5);
  connection->receive(rx_buffer,22);
  if(rx_buffer[4]){
    throw Nxt_exception("read","I2C", 0x00ff & rx_buffer[4]);
  }
  i=0;
  //printf("\nI2C read:\n");
  while(i<16){
     rx_buffer[i]=rx_buffer[i+5];
     //printf("read[%d]: %d\n", i, rx_buffer[i]);
     i++;
  }
}

void I2c::wait_for_bytes(int num_bytes){
  bytes_ready=0;
  time_t start_time = clock();
  do{
    update_sensor_status();
    Sleep(wait_time);
    if(((clock()-start_time) > TIMEOUT)){
      throw Nxt_exception("wait_for_bytes","I2C", NXT_I2C_TIMEOUT);
    }
    }while(bytes_ready < num_bytes);
}

void I2c::update_sensor_status(){
  unsigned char command[5];
  unsigned char answer[NXT_BUFFER_SIZE];
  command[0]=0x03;
  command[1]=0x00;

  command[2]=0x00;
  command[3]=0x0E;
  command[4]=sensor_port;
  connection->send(&command[0],5);
  connection->receive(&answer[0],6);
  if(answer[4]){
    if(answer[4] != NXT_PENDING_COMMUNICATION){
      throw Nxt_exception("update_sensor_status", "I2C", 0x00ff & answer[4]);
    }
    bytes_ready = 0;
  }
  else{
    bytes_ready=answer[5];
  }
}

unsigned char I2c::get_i2c_address(){
  return this->i2c_address;
}





