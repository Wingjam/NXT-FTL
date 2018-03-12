#include "stdafx.h"
#include "brick.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <windows.h>
#include "error.h"

#define MSG_SIZE 64


Brick::Brick(Connection *connection){
  this->connection = connection;
}

void Brick::write_msg(string message, int inbox, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[NXT_BUFFER_SIZE];
  unsigned int i=0;
  unsigned int length=message.length();
  if(length>57){
     length = 57;
  }
  command[0]=length+5;  //command length
  command[1]=0x00;
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x09;
  if(inbox>9){
    inbox=9;
  }
  command[4]=inbox;
  command[5]=length+1;
  while(i<length){
    command[i+6]=message[i];
    i++;
  }
  command[i+6]='\0';
  connection->send(&command[0],length+7);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("write_msg","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

string Brick::read_msg(int inbox, bool remove){
  unsigned int i;
  unsigned char answer[NXT_BUFFER_SIZE+2];
  unsigned char command[7];
  command[0]=0x05;  //command length
  command[1]=0x00;

  command[2]=0x00;
  command[3]=0x13;
  command[4]=10+inbox;
  command[5]=0x00;
  command[6]=remove;
  connection->send(&command[0],7);
  connection->receive(&answer[0],66);
  if(answer[4]){
    throw Nxt_exception("read_msg","Brick", answer[4]);
  }
  i=7;
  string result;
  result.resize(59);
  while((answer[i]!='\0')){
    result[i-7]=answer[i];
    i++;
  }
  result[i-7]='\0';
  //connection->flush();
  return result;
}

void Brick::set_name(string name, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[NXT_BUFFER_SIZE];
  unsigned int length=name.length();
  unsigned int i;
  if(length>15){
    length=15;
  }
  command[0]=length+3;  //command length
  command[1]=0x00;

  //start of message
  if(reply){
    command[2] = 0x01;
  }
  else{
    command[2] = 0x81;
  }
  command[3]=0x98;
  for(i=0; i<length ;i++){
    command[i+4]=name[i];
  }
  command[i+4]='\0';
  connection->send(&command[0],length+5);
  Sleep(200);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("set_name","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

void Brick::get_device_info(Device_info &info){
  unsigned int i;
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0] = 0x02;
  command[1] = 0x00;
  command[2] = 0x01;
  command[3] = 0x9b;
  connection->send(&command[0],4);
  connection->receive(&answer[0],35);
  if(answer[4]){
    throw Nxt_exception("get_device_info","Brick", 0x00FF & answer[4]);
  }
  info.name.resize(15);
  i=0;
  while(i<15){
    info.name[i]=answer[i+5];
    i++;
  }
  info.bt_address.resize(21);
  i=0;
  while(i<7){
    sprintf(&info.bt_address[i*3], "%02X:", answer[i+20]);
    i++;
  }
  info.bt_address[20] = '\0';
  //LEGO has not implemented Bluetooth strengh
  //bt_strength = ((0xff & answer[27]) | ( answer[28] << 8) | (answer[29] << 16) | (answer[30] << 24) );
   info.flash_memory = ((0xff & answer[31]) | ( answer[32] << 8) | ( answer[33] << 16) | ( answer[34] << 24) );
   return;
}

string Brick::get_name(){
  this->get_device_info(this->info);
  return this->info.name;
}

void Brick::keep_alive(bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x0D;
  connection->send(&command[0],4);
  if(reply){
    connection->receive(&answer[0],9);
    if(answer[4]){
      throw Nxt_exception("keep_alive","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

void Brick::play_tone(unsigned int freq, unsigned int time, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[8];
  command[0]=0x06;  //command length
  command[1]=0x00;
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x03;
  command[4]=freq;
  command[5]=freq >>8;
  command[6]=time;
  command[7]=time >> 8;
  connection->send(&command[0],8);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("play_tone","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

void Brick::beep(unsigned int time, bool reply){
  play_tone(1000,time, reply);
}

void Brick::play_soundfile(string file, bool loop, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[NXT_BUFFER_SIZE];
  unsigned int length=file.length();
  unsigned int i;
  if(length>19){
    length=19;
  }
  command[0]=length+4;  //command length
  command[1]=0x00;
  //start of message
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x02;
  command[4]=loop;
  for(i=0; i<length ;i++){
    command[i+5]=file[i];
  }
  command[i+5]='\0';
  connection->send(&command[0],length+6);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("play_soundfile","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

void Brick::stop_soundplayback(bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x0C;
  connection->send(&command[0],4);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("stop_soundplayback","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

void Brick::start_program(string file, bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[NXT_BUFFER_SIZE];
  int length=file.length();
  int i;
  if(length>19){
    length=19;
  }
  command[0]=length+3;  //command length
  command[1]=0x00;
  //start of message
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x00;
  for(i=0; i<length ;i++){
    command[i+4]=file[i];
  }
  command[i+4]='\0';
  connection->send(&command[0],length+5);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("start_program","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

void Brick::stop_programs(bool reply){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;
  if(reply){
    command[2]=0x00;
  }
  else{
    command[2]=0x80;
  }
  command[3]=0x01;
  connection->send(&command[0],4);
  if(reply){
    connection->receive(&answer[0],5);
    if(answer[4]){
      throw Nxt_exception("stop_programs","Brick", 0x00FF & answer[4]);
    }
  }
  return;
}

string Brick::get_current_program(){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  string temp;
  unsigned int i;
  command[0]=0x02;
  command[1]=0;
  command[2]=0x00;
  command[3]=0x11;
  connection->send(&command[0],4);
  connection->receive(&answer[0],25);
  if(answer[4]){
      throw Nxt_exception("get_current_program","Brick", 0x00FF & answer[4]);
  }
  i=5;
  temp.resize(20);
  while(i<25){
    temp[i-5]=answer[i];
    i++;
  }
  temp[i-5]='\0';
  return temp;
}

string Brick::get_bt_address(){
  this->get_device_info(this->info);
  return this->info.bt_address;
}

void Brick::get_device_version(Device_version &version){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;

  command[2]=0x01;
  command[3]=0x88;
  connection->send(&command[0],4);
  connection->receive(&answer[0],9);
  if(answer[4]){
    throw Nxt_exception("get_device_version","Brick", 0x00FF & answer[4]);
  }
  version.protocol.resize(10);
  version.protocol[0] = answer[6]+48;
  version.protocol[1] = '.';
  sprintf(&version.protocol[2], "%d", answer[5]);
  version.firmware.resize(10);
  version.firmware[0]=answer[8]+48;
  version.firmware[1]='.';
  sprintf(&version.firmware[2], "%02d", answer[7]);
  //Sleep(80); //For some reason it is not possible to recieve a system command and send a direct command without delay
  return;
}

string Brick::get_firmware_version(){
  this->get_device_version(this->version);
  return this->version.firmware;
}

string Brick::get_protocol_version(){
  this->get_device_version(this->version);
  return this->version.protocol;
}

unsigned int Brick::get_flash_memory(){
  this->get_device_info(this->info);
  return this->info.flash_memory;
}

unsigned int Brick::get_battery_level(){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  int mvolt;
  command[0]=0x02;  //command length
  command[1]=0x00;

  command[2]=0x00;
  command[3]=0x0B;
  connection->send(&command[0],4);
  connection->receive(&answer[0],7);
  if(answer[4]){
    throw Nxt_exception("battery_level","Brick", answer[4]);
  }
  mvolt = ((0xFF & answer[5]) | (answer[6] << 8));
  //Sleep(80); //For some reason it is not possible to recieve a system command and send a direct command without delay
  return mvolt;
}

// LEGO skiped the implementation
//unsigned int Brick::get_bt_strength(){}

/*






    string Brick::get_bt_address(){
      this->get_info();
      return this->bt_address;
    }


    void Brick::delete_flash(bool reply){
      char answer[NXT_BUFFER_SIZE];
      char command[4];
      command[0] = 0x02;
      command[1] = 0x00;
      if(reply){
        command[2]=0x01;
      }
      else{
        command[2]=0x81;
      }
      command[3] = 0xA0;
      connection->send(&command[0],4);
      if(reply){
        Sleep(3000);//wait for NXT
        connection->receive(&answer[0],5);
        if(answer[4]){
           throw Nxt_exception("delete_flash","Brick", 0x00FF & answer[4]);
        }
      }


    }






    void Brick::download_file(string pc_file, string nxt_file){
       FILE *to;
       unsigned int i,file_size;
       char *data;
       NXT_file *from = new NXT_file(this->connection);
       if((to =fopen(pc_file.c_str(),"wb"))==NULL){
         throw Nxt_exception("download_file","Brick", PC_FILE_ERROR);
       }
       file_size = from->open(nxt_file,READ);
       data =(char *) malloc(file_size*sizeof(char));
       from->read(data,file_size);
       i=0;
       while(i<file_size){
         fputc(data[i], to);
         i++;
       }
       free(data);
       fclose(to);
       from->close();
       delete(from);
       return;
    }

    void Brick::delete_file(string file_name, bool reply){
      unsigned int name_length;
      char command[24];
      char answer[NXT_BUFFER_SIZE];
      unsigned int i=0;
      name_length = file_name.length();
      if(name_length > MAX_NAME_LENGTH){
        file_name.erase(MAX_NAME_LENGTH+1);
        name_length=MAX_NAME_LENGTH;
      }
      command[0]=0x16;  //command length
      command[1]=0x00;
      if(reply){
        command[2]=0x01;
      }
      else{
        command[2]=0x81;
      }
      command[3]=0x85;
      while(i<name_length){
        command[i+4]=file_name[i];
        i++;
      }
      while(i<MAX_NAME_LENGTH){
        command[i+4]='\0';
        i++;
      }
      connection->send(&command[0],24);
      if(reply){
        connection->receive(&answer[0],25);
        if(answer[4]){
           throw Nxt_exception("delete_file","Brick", 0x00FF & answer[4]);
        }
      }
    }

void Brick::upload_file(string pc_file, string nxt_file){
  FILE *from;
  unsigned int i,file_size;
  char *data;
  string file_ext;
  NXT_file *to = new NXT_file(this->connection);
  if((from =fopen(pc_file.c_str(),"rb"))==NULL){
    throw Nxt_exception("upload_file","Brick", PC_FILE_ERROR);
  }
  fseek( from, 0L, SEEK_END );
  file_size = ftell( from );
  fseek( from, 0, SEEK_SET);

  file_ext = nxt_file.substr( nxt_file.find_last_of(".")+1);
  i=0;
  while(i<file_ext.length()){
    file_ext[i] = std::tolower(file_ext[i]);
    i++;
  }
  if(file_ext == "ric" || file_ext == "rxe"){
     to->open(nxt_file, WRITE_LINEAR, file_size);
  }
  else{
     to->open(nxt_file,WRITE,file_size);
  }
  data = (char *) malloc(file_size * sizeof(char));
  i=0;
  while(i<file_size){
    data[i] = fgetc(from);
    i++;
  }
  to->write(data, file_size);
  free(data);
  fclose(from);
  to->close();
  delete(to);
  return;
}

int Brick::get_first(string wild_card, file_info *info){
  char answer[NXT_BUFFER_SIZE];
  char command[24];
  unsigned int i=0;
  unsigned int name_length = wild_card.length();
  if(name_length > MAX_NAME_LENGTH){
    wild_card.erase(MAX_NAME_LENGTH+1);
    name_length=MAX_NAME_LENGTH;
  }
  command[0]=0x16;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=0x86;
  while(i<name_length){
    command[i+4]=wild_card[i];
    i++;
  }
  while(i<MAX_NAME_LENGTH){
    command[i+4]='\0';
    i++;
  }
  connection->send(&command[0],24);
  connection->receive(&answer[0],30);
  if(answer[4]){
     if(answer[4]!= (char) FILE_NOT_FOUND){
       throw Nxt_exception("get_first","Brick", 0x00FF & answer[4]);
     }
     else{
       return 0;
     }
  }
  info->handle = answer[5];
  i=0;
  while(i<MAX_NAME_LENGTH){
    info->name[i] = answer[6+i];
    i++;
  }
  info->size = (0xFF & answer[26]) | ((0xFF & answer[27]) << 8)| ((0xFF & answer[28]) << 16)| ((0xFF & answer[29]) << 24);
  //close the handle
  command[0] = 0x03;
  command[1] = 0x00;
  command[2] = 0x01;
  command[3] = 0x84;
  command[4] = answer[5];
  connection->send(&command[0],5);
  connection->receive(&answer[0],6);
  if(answer[4]){
    if(answer[4] != (char) HANDLE_ALREADY_CLOSED){
      throw Nxt_exception("get_first","Brick", 0x00FF & answer[4]);
    }
  }
  return 1;
}

int Brick::get_next(int handle, file_info *info){
  char answer[NXT_BUFFER_SIZE];
  char command[5];
  unsigned int i=0;
  command[0] = 0x03;
  command[1] = 0x00;
  command[2] = 0x01;
  command[3] = 0x87;
  command[4] = handle;
  connection->send(&command[0],5);
  connection->receive(&answer[0],30);
  if(answer[4]){
    if(answer[4]!= (char) FILE_NOT_FOUND){
       throw Nxt_exception("get_next","Brick", 0x00FF & answer[4]);
     }
     else{
       return 0;
     }
  }
  info->handle = answer[5];
  i=0;
  while(i<MAX_NAME_LENGTH){
    info->name[i] = answer[6+i];
    i++;
  }
  info->size = (0xFF & answer[26]) | ((0xFF & answer[27]) << 8)| ((0xFF & answer[28]) << 16)| ((0xFF & answer[29]) << 24);
  //close the handle
  command[0] = 0x03;
  command[1] = 0x00;
  command[2] = 0x01;
  command[3] = 0x84;
  command[4] = answer[5];
  connection->send(&command[0],5);
  connection->receive(&answer[0],6);
  if(answer[4]){
    if(answer[4] != (char) HANDLE_ALREADY_CLOSED){
      throw Nxt_exception("get_next","Brick", 0x00FF & answer[4]);
    }
  }
  return 1;
}

void Brick::get_file_list(string wild_card, file_list *list){
  unsigned int idx;
  file_info temp;
  free(list->file);
  if(this->get_first(wild_card, &temp)){
     list->file = (file_info*) malloc(sizeof(file_info));
     list->file[0] = temp;
     list->length = 1;
  }
  else{
    list->length = 0;
    return;
  }
  idx=0;
  while(this->get_next(list->file[idx].handle, &temp)){
    list->file = (file_info *) realloc(list->file, (list->length+1)* sizeof(file_info));
    list->file[idx+1] = temp;
    list->length++;
    idx++;
  }
  return;
}


*/
