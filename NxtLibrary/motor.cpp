#include "stdafx.h"
#include <iostream>
#include "motor.h"
#include <stdio.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include "error.h"
using namespace std;


Motor::Motor(Motor_port port, Connection *connection){
  this->port=port;
  this->connection = connection;
  //mode = BREAK | REGULATED;
  //reg_mode = MOTOR_SPEED;
  //run_state = RUN_IDLE;
  //turnratio = 100;
}

void Motor::get_output_state(){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[5];
  command[0]=0x03;  //command length
  command[1]=0x00;

  //start of message
  command[2]=0x00;
  command[3]=0x06;
  command[4]=port;
  connection->send(&command[0],5);
  connection->receive(&answer[0],27);
  if(answer[4]){
    throw Nxt_exception("get_output_state","Motor", answer[4]);
  }
  //mode=answer[7];
  //reg_mode=answer[8];
  //turnratio=answer[9];
  run_state=answer[10];
  tacho_limit = (0xFF & answer[11]) | ((0xFF & answer[12]) << 8)| ((0xFF & answer[13]) << 16)| ((0xFF & answer[14]) << 24);
  tacho_count = (0xFF & answer[15]) | ((0xFF & answer[16]) << 8)| ((0xFF & answer[17]) << 16)| ((0xFF & answer[18]) << 24);
  block_count = (0xFF & answer[19]) | ((0xFF & answer[20]) << 8)| ((0xFF & answer[21]) << 16)| ((0xFF & answer[22]) << 24);
  rotation_count = (0xFF & answer[19]) | ((0xFF & answer[20]) << 8)| ((0xFF & answer[21]) << 16)| ((0xFF & answer[22]) << 24);
  //printf("Run state: %d\n", run_state);
  //printf("Tacho limit: %d\n", tacho_limit);
  //printf("Tacho count: %d\n", tacho_count);
  //printf("Block tacho count: %d\n", block_count);
  //printf("Rotation count: %d\n", rotation_count);
  //printf("\n");
  return;
}

void Motor::set_output_state(char set_speed, unsigned char set_mode, unsigned char set_regulation, char set_turn_ratio, unsigned char set_run_state, unsigned int set_tacho_limit, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[15];
  command[0]=13;  //command length
  command[1]=0x00;

  //start of message
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x04;//Turn on motor
  command[4]=port;//Output port
  command[5]=set_speed;//power set point
  command[6]=set_mode;//Mode
  command[7]=set_regulation;//Regulation
  command[8]=set_turn_ratio;//Turn ratio
  command[9]=set_run_state;//Run stat
  command[10]=set_tacho_limit;//TachoLimit
  command[11]=set_tacho_limit >> 8;//TachoLimit
  command[12]=set_tacho_limit >> 16;//TachoLimit
  command[13]=set_tacho_limit >> 24;//TachoLimit
  command[14]=0;                   // why a 5th byte?????
  connection->send(&command[0],15);
  //connection->send(&command[0],8);

  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("set_output_state","Motor", 0x00FF & answer[4]);
    }
  }
  return;
}

void Motor::reset(bool relative, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[6];
  command[0]=0x04;  //command length
  command[1]=0x00;

  //start of message
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x0A;
  command[4]=port;
  command[5]=relative;
  connection->send(&command[0],6);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("reset","Motor", 0x00FF & answer[4]);
    }
  }
  return;
}

void Motor::on(char speed, unsigned int degrees, bool reply){
  return set_output_state(speed, BREAK | MOTOR_ON | REGULATED, MOTOR_SPEED, DEFAULT_TURN_RATIO, RUNNING, degrees, reply);
}

void Motor::stop(bool reply){
  return set_output_state(0, BREAK | MOTOR_ON | REGULATED, MOTOR_SPEED, DEFAULT_TURN_RATIO, RUNNING, 0, reply);
}

void Motor::off(bool reply){
  return set_output_state(0, REGULATED, MOTOR_SPEED, DEFAULT_TURN_RATIO, RUN_IDLE, 0, reply);
}
void Motor::reset_rotation(bool reply){
  return reset(true, reply);
}
bool Motor::is_running(){
  get_output_state();
  if(run_state){
    return true;
  }
  else
    return false;
}

long int Motor::get_rotation(){
  get_output_state();
  return rotation_count;
}

void Motor::move_to(char speed, long int target){
  return move_to(speed,target,4);
}

void Motor::move_to(char speed, long int target, int tol){
  speed=abs(speed);
  long int current_pos= get_rotation();
  if(target>current_pos){
    on(speed);
    while(get_rotation()<=target){
      //wait
    }
    stop();
  }
  else{
    on(-speed);
    while(get_rotation()>=target){
      //wait
    }
    stop();
  }
  if( abs(target-get_rotation()) >tol ){
    move_to(5,target);
  }
  return;
}

Motor_port Motor::get_port(){
   return this->port;
}

/********* Work in progress******/


/*    int Motor::move_to(int speed, long int target){
        speed=abs(speed);
        int dir=1;
        long int slow_down=720;
        long int current_pos=get_rotation();
        if(target<current_pos){
            dir=-1;
        }
        printf("target:%d\n",target);
        on(speed * dir);
        do{
             current_pos=get_rotation();
                 printf("Pos1 %d\n", current_pos);
                 }while(target-slow_down > current_pos);
        set_output_state(0, BREAK | MOTOR_ON | REGULATED, reg_mode, turnratio, RAMP_DOWN, slow_down );
        do{
                 current_pos=get_rotation();
                 }while(target-15>current_pos);
        set_output_state(0, BREAK | MOTOR_ON| REGULATED, reg_mode, turnratio, RUNNING, 0);
        if(target<current_pos){
            dir=-1;
        }
        else{
            dir=1;
        }
        on(10*dir);
        do{
                 current_pos=get_rotation();
        }while(target>current_pos);


    }*/

