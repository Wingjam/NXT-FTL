#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <windows.h>
#include "error.h"
#include "brick.h"

//#define MAX_WRITE_BYTES 0x00ff
//#define MAX_READ_BYTES 0xffff


#define MAX_WRITE_BYTES 0x159A
#define MAX_READ_BYTES 0x0800

//#define MAX_WRITE_BYTES 0x3AAA
//#define MAX_READ_BYTES 0xffff

#define OPEN_WRITE 0x81
#define OPEN_WRITE_LINEAR 0x89
#define OPEN_WRITE_LINEAR_DATA 0x8B


Filesystem::Filesystem(Connection *connection){
  this->connection = connection;
  this->file_list_size = 0;
}
Filesystem::~Filesystem(){
  free(this->file_list);
}

//fix bug with file left open when a exception is thrown
void Filesystem::download_file(string pc_name, string nxt_name){
  FILE *to;
  unsigned int i,file_size;
  unsigned char *data;
  Nxt_file from;
  if((to =fopen(pc_name.c_str(),"wb"))==NULL){
    throw Nxt_exception("download_file","Filesystem", PC_FILE_ERROR);
  }
  from = this->open_file(nxt_name, READ);
  file_size = from.size;
  data =(unsigned char *) malloc(file_size*sizeof(unsigned char));
  read_file(from, data, file_size);
  i=0;
  while(i<file_size){
    fputc(data[i], to);
    i++;
  }
  free(data);
  fclose(to);
  close_file(from);
  return;
}

void Filesystem::delete_file(string file_name, bool reply){
  unsigned int name_length;
  unsigned char command[24];
  unsigned char answer[NXT_BUFFER_SIZE];
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
      throw Nxt_exception("delete_file","Filesystem", 0x00FF & answer[4]);
    }
  }
}

//fix bug with file left open when a exception is thrown
void Filesystem::upload_file(string pc_name, string nxt_name){
  FILE *from;
  unsigned int i,file_size;
  unsigned char *data;
  string file_ext;
  Nxt_file to;
  if((from =fopen(pc_name.c_str(),"rb"))==NULL){
    throw Nxt_exception("upload_file","Filesystem", PC_FILE_ERROR);
  }
  fseek( from, 0L, SEEK_END );
  file_size = ftell( from );
  fseek( from, 0, SEEK_SET);

  file_ext = nxt_name.substr( nxt_name.find_last_of(".")+1);
  i=0;
  while(i<file_ext.length()){
    file_ext[i] = std::tolower(file_ext[i]);
    i++;
  }
  if(file_ext == "ric" || file_ext == "rxe"){
     to = open_file(nxt_name, WRITE_LINEAR, file_size);
  }
  else{
     to = open_file(nxt_name, WRITE, file_size);
  }
  data = (unsigned char *) malloc(file_size * sizeof(unsigned char));
  i=0;
  while(i<file_size){
    data[i] = fgetc(from);
    i++;
  }
  write_file(to, data, file_size);
  free(data);
  fclose(from);
  close_file(to);
  return;
}

int Filesystem::get_first(string wild_card, Nxt_file *file){
  Nxt_file temp;
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[24];
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
     if((unsigned int) answer[4]!=  NXT_FILE_NOT_FOUND){
       throw Nxt_exception("get_first","Filesystem", 0x00FF & answer[4]);
     }
     else{
       return 0;
     }
  }
  file->handle = answer[5];
  i=0;
  while(i<MAX_NAME_LENGTH){
    file->name[i] = answer[6+i];
    //printf("command[%d]: 0x%x\n", i+6, answer[i+6]);
    i++;
  }
  //cout << file->name << endl;
  file->size = (0xFF & answer[26]) | ((0xFF & answer[27]) << 8)| ((0xFF & answer[28]) << 16)| ((0xFF & answer[29]) << 24);
  temp.handle = file->handle;
  //printf("\nClosing file\n");
  try{
    close_file(temp);
  }
  catch (Nxt_exception& e){
    if( e.error_code() != NXT_HANDLE_ALREADY_CLOSED){
      throw Nxt_exception("get_first","Filesystem", 0x00FF & answer[4]);
    }
  }
  return 1;
}

int Filesystem::get_next(int handle, Nxt_file *file){
  Nxt_file temp;
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[5];
  unsigned int i=0;
  command[0] = 0x03;
  command[1] = 0x00;
  command[2] = 0x01;
  command[3] = 0x87;
  command[4] = handle;
  connection->send(&command[0],5);
  connection->receive(&answer[0],30);
  if(answer[4]){
    if( (unsigned int) answer[4]!=  NXT_FILE_NOT_FOUND){
       throw Nxt_exception("get_next","Filesystem", 0x00FF & answer[4]);
     }
     else{
       return 0;
     }
  }
  file->handle = answer[5];
  i=0;
  while(i<MAX_NAME_LENGTH){
    file->name[i] = answer[6+i];
    i++;
  }
  file->size = (0xFF & answer[26]) | ((0xFF & answer[27]) << 8)| ((0xFF & answer[28]) << 16)| ((0xFF & answer[29]) << 24);
  temp.handle = file->handle;
  try{
    close_file(temp);
  }
  catch (Nxt_exception& e){
    if(e.error_code () != NXT_HANDLE_ALREADY_CLOSED){
      throw Nxt_exception("get_next","Filesystem", 0x00FF & answer[4]);
    }
  }
  return 1;
}

unsigned int Filesystem::update_file_list(string wild_card){
  unsigned int idx;
  Nxt_file temp;
  free(file_list);
  if(this->get_first(wild_card, &temp)){
     file_list = (Nxt_file*) malloc(sizeof(Nxt_file));
     file_list[0] = temp;
     file_list_size = 1;
  }
  else{
    file_list_size =0;
    return file_list_size;
  }
  idx=0;
  while(this->get_next(file_list[idx].handle, &temp)){
    file_list = (Nxt_file *) realloc(file_list, (file_list_size+1)* sizeof(Nxt_file));
    file_list[idx+1] = temp;
    file_list_size++;
    idx++;
  }
  return file_list_size;
}
unsigned int Filesystem::create_file_list(string wild_card){
  return update_file_list(wild_card);
}

unsigned int Filesystem::get_file_list_size(){
  return file_list_size;
}

Nxt_file Filesystem::get_file_list_element(unsigned int idx){
  if(idx>=file_list_size){
     Nxt_file empty;
     empty.handle =0;
     empty.name[0] = '\0';
     empty.size =0;
     return empty;
  }
  else{
     return file_list[idx];
  }
}

//This is so ugly
unsigned int Filesystem::get_free_flash(){
   Brick *temp = new Brick(this->connection);
   unsigned int temp2 = temp->get_flash_memory();
   delete(temp);
   return temp2;
}

Nxt_file Filesystem::open_read(string file_name){
  Nxt_file file;
  unsigned int name_length;
  unsigned char command[24];
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned int i=0;
  name_length = file_name.length();
  if(name_length > MAX_NAME_LENGTH){
    file_name.erase(MAX_NAME_LENGTH+1);
    name_length=MAX_NAME_LENGTH;
  }
  command[0]=0x16;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=0x80;
  while(i<name_length){
    command[i+4]=file_name[i];
    i++;
  }
  while(i<MAX_NAME_LENGTH){
    command[i+4]='\0';
    i++;
  }
  connection->send(&command[0],24);
  connection->receive(&answer[0],10);
  if(answer[4]){
     throw Nxt_exception("open_read","Filesystem", 0x00FF & answer[4]);
  }
  file.handle = answer[5];
  file.size = (0xFF & answer[6]) | ((0xFF & answer[7]) << 8)| ((0xFF & answer[8]) << 16)| ((0xFF & answer[9]) << 24);
  i=0;
  while(i<MAX_NAME_LENGTH){
    file.name[i] = file_name[i];
    i++;
  }
  return file;
}

Nxt_file Filesystem::open_append(string file_name){
  Nxt_file file;
  unsigned int name_length;
  unsigned char command[24];
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned int i=0;
  name_length = file_name.length();
  if(name_length > MAX_NAME_LENGTH){
    file_name.erase(MAX_NAME_LENGTH+1);
    name_length=MAX_NAME_LENGTH;
  }
  command[0]=0x16;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=0x8C;
  while(i<name_length){
    command[i+4]=file_name[i];
    i++;
  }
  while(i<MAX_NAME_LENGTH){
    command[i+4]='\0';
    i++;
  }
  connection->send(&command[0],24);
  connection->receive(&answer[0],10);
  if(answer[4]){
     throw Nxt_exception("open_append","Filesystem", 0x00FF & answer[4]);
  }
  file.handle = answer[5];
  file.size = (0xFF & answer[6]) | ((0xFF & answer[7]) << 8)| ((0xFF & answer[8]) << 16)| ((0xFF & answer[9]) << 24);
  i=0;
  while(i<MAX_NAME_LENGTH){
    file.name[i] = file_name[i];
    i++;
  }
  return file;
}

Nxt_file Filesystem::open_write(string file_name, unsigned int file_size, char type){
  Nxt_file file;
  unsigned int name_length;
  unsigned char command[28];
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned int i=0;
  name_length = file_name.length();
  if(name_length > MAX_NAME_LENGTH){
    file_name.erase(MAX_NAME_LENGTH+1);
    name_length=MAX_NAME_LENGTH;
  }
  command[0]=0x1A;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=type;
  while(i<name_length){
    command[i+4]=file_name[i];
    i++;
  }
  while(i<MAX_NAME_LENGTH){
    command[i+4]='\0';
    i++;
  }
  command[24] = file_size & 0xff;
  command[25] = (file_size >> 8 ) & 0xff;
  command[26] = (file_size >> 16) & 0xff;
  command[27] = (file_size >> 24) & 0xff;
  connection->send(&command[0],28);
  connection->receive(&answer[0],6);
  if(answer[4]){
     throw Nxt_exception("open_write","Filesystem", 0x00FF & answer[4]);
  }
  file.handle = answer[5];
  file.size = file_size;
  i=0;
  while(i<MAX_NAME_LENGTH){
    file.name[i] = file_name[i];
    i++;
  }
  return file;
}

void Filesystem::close_file(Nxt_file &file){
  unsigned char command[5];
  unsigned char answer[NXT_BUFFER_SIZE];
  command[0]=0x03;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=0x84;
  command[4]= file.handle;

  connection->send(&command[0],5);
  connection->receive(&answer[0],6);
  if(answer[4]){
     throw Nxt_exception("close","Filesystem", 0x00FF & answer[4]);
  }
  file.name[0] = '\0';
  file.handle = 0;
  file.size = 0;
  return;
}

unsigned int Filesystem::read_file(Nxt_file &file, unsigned char *buffer, unsigned int num_bytes){
  unsigned short int bytes_to_read=0;
  unsigned int i,j;
  i=num_bytes/MAX_READ_BYTES;
  j=0;
  while(j<i){
    this->read_command(file, &buffer[j*MAX_READ_BYTES], MAX_READ_BYTES);
    j++;
  }
  bytes_to_read = num_bytes%MAX_READ_BYTES;
  if(bytes_to_read){
    this->read_command(file, &buffer[i*MAX_READ_BYTES],bytes_to_read);
  }
  return bytes_to_read + (i*MAX_READ_BYTES);
}

unsigned short int Filesystem::read_command(Nxt_file &file, unsigned char *buffer, unsigned short int num_bytes){
  unsigned char command[7];
  unsigned char *answer = (unsigned char *) malloc((num_bytes*sizeof(unsigned char))+8);
  unsigned short int bytes_to_read,i;
  command[0]=0x05;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=0x82;
  command[4]= file.handle;
  command[5] = num_bytes & 0x00ff;
  command[6] = (num_bytes >> 8 & 0x00ff);
  connection->send(&command[0],7);
  connection->receive(&answer[0],8+num_bytes);
  if(answer[4]){
    throw Nxt_exception("read_command","Filesystem", 0x00FF & answer[4]);
  }
  bytes_to_read = (0xFF & answer[6]) | ((0xFF & answer[7]) << 8);
  if(bytes_to_read != (num_bytes)){
    throw Nxt_exception("read_command","Filesystem", NXT_UNDEFINED_FILE_ERROR);
  }
  i=0;
  while(i<num_bytes){
    buffer[i]=answer[8+i];
    i++;
  }
  free(answer);
  return bytes_to_read;
}

//returns bytes wtritten
unsigned int Filesystem::write_file(Nxt_file &file, unsigned char *buffer, unsigned int num_bytes){
  unsigned short int bytes_to_write=0;
  unsigned int i,j;
  i=num_bytes/MAX_WRITE_BYTES;
  j=0;
  while(j<i){
    this->write_command(file, &buffer[j*MAX_WRITE_BYTES], MAX_WRITE_BYTES);
    j++;
  }
  bytes_to_write = num_bytes%MAX_WRITE_BYTES;
  if(bytes_to_write){
    this->write_command(file, &buffer[i*MAX_WRITE_BYTES],bytes_to_write);
  }
  return bytes_to_write + (i*MAX_WRITE_BYTES);
}

unsigned short int Filesystem::write_command(Nxt_file &file, unsigned char *buffer, unsigned short int num_bytes){
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned int i;
  unsigned short int bytes_to_write, bytes_written;
  if(num_bytes>MAX_WRITE_BYTES){
     return 0;
  }
  bytes_to_write = num_bytes + 3;
  unsigned char *command = (unsigned char *) malloc ( (num_bytes*sizeof(unsigned char))+5);
  command[0]= bytes_to_write & 0xff;  //command length
  command[1]= (bytes_to_write >> 8) & 0xff;
  command[2]=0x01;
  command[3]=0x83;
  command[4]= file.handle;
  i=0;
  while(i<num_bytes){
    command[i+5]=buffer[i];
    i++;
  }
  connection->send(&command[0], num_bytes+5);
  //Sleep(1000);
  connection->receive(&answer[0],8);
  free(command);
  if(answer[4]){
    throw Nxt_exception("write_command","Filesystem", 0x00FF & answer[4]);
  }
  bytes_written = (0xFF & answer[6]) | ((0xFF & answer[7]) << 8);
  if(bytes_written != (num_bytes)){
    throw Nxt_exception("write_command","Filesystem", NXT_UNDEFINED_FILE_ERROR);
  }
  return bytes_written;
}

Nxt_file Filesystem::open_file(string file_name, File_mode mode, unsigned int file_size ){
   Nxt_file file;
   if(mode == READ){
     return open_read(file_name);
   }
   if(mode == WRITE){
     return open_write(file_name, file_size, OPEN_WRITE);
   }
   if(mode == WRITE_LINEAR){
       return open_write(file_name, file_size, OPEN_WRITE_LINEAR);
   }
   if(mode==WRITE_LINEAR_DATA){
      return open_write(file_name, file_size, OPEN_WRITE_LINEAR_DATA);
   }
   if(mode== APPEND){
     return open_append(file_name);
   }
   file.handle=0;
   file.name[0] = '\0';
   file.size = 0;
   return file;
}

void Filesystem::delete_file(Nxt_file &file, bool reply){
   return this->delete_file(file.name, reply);
}

void Filesystem::delete_flash(){
  unsigned char command[4];
  unsigned char answer[NXT_BUFFER_SIZE];
  command[0]=0x02;  //command length
  command[1]=0x00;
  command[2]=0x01;
  command[3]=0xA0;

  connection->send(&command[0],4);
  //if(!connection->get_type() == NXT_NETWORK){
  Sleep(3000);
    //sleep is done on the server over network
  //}
  connection->receive(&answer[0],5);
  if(answer[4]){
     throw Nxt_exception("delete_flash","Filesystem", 0x00FF & answer[4]);
  }
  return;
}
