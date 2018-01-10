#include "error.h"
#include <sstream>
#include <string>

using namespace std;

Nxt_exception::Nxt_exception(string function_name, string class_name, unsigned int error_code, string description){
   this->function_name = function_name;
   this->class_name = class_name;
   this_error_code = error_code;
   this->description = description;
}

Nxt_exception::~Nxt_exception() throw(){

}

unsigned int Nxt_exception::error_code(){
  return this_error_code;
}

const char* Nxt_exception::what() const throw(){
  if(description.empty()){
    return (error_info(this_error_code));
  }
  else{
    return description.c_str();
  }
}

const std::string Nxt_exception::who(){
   return ("Function \"" +function_name + "\" in class \"" + class_name + "\"");
}

Error_type Nxt_exception::error_type(){
  if(this_error_code >= SERVER_ERROR_LOW && this_error_code <= SERVER_ERROR_HIGH){
    return SERVER_ERROR;
  }

  if(this_error_code >= NXT_ERROR_LOW && this_error_code <= NXT_ERROR_HIGH){
    return NXT_ERROR;
  }

  if(this_error_code >= BT_ERROR_LOW && this_error_code <= BT_ERROR_HIGH){
    return BT_ERROR;
  }

  if(this_error_code >= PC_FILE_ERROR_LOW && this_error_code <= PC_FILE_ERROR_HIGH){
    return PC_FILE_ERROR;
  }

  if(this_error_code >= NETWORK_ERROR_LOW && this_error_code <= NETWORK_ERROR_HIGH){
    return NETWORK_ERROR;
  }
  //default
  return UNKNOWN_ERROR;
}



const char * Nxt_exception::error_info(unsigned int error_code) const{
  //can not use a switch .-)
  if(error_code == SERVER_BANNED){
    return  SERVER_BANNED_TEXT;
  }
  if(error_code == SERVER_BT_BUSSY){
    return  SERVER_BT_BUSSY_TEXT;
  }
  if(error_code == SERVER_PASSWORD_FAIL){
    return  SERVER_PASSWORD_FAIL_TEXT;
  }
  if(error_code == SERVER_BT_ERROR){
    return SERVER_BT_ERROR_TEXT;
  }
  if(error_code == SERVER_BT_NOT_ENOUGH_BYTES){
    return SERVER_BT_NOT_ENOUGH_BYTES_TEXT;
  }
  if(error_code == NETWORK_COM_ERROR){
    return NETWORK_COM_ERROR_TEXT;
  }
  if(error_code == SERVER_LOST_NXT){
    return SERVER_LOST_NXT_TEXT;
  }
  if(error_code == SERVER_FULL){
    return SERVER_FULL_TEXT;
  }
  if(error_code ==  BT_INVALID_COM_PORT){
    return  BT_INVALID_COM_PORT_TEXT;
  }
  if(error_code ==  BT_ERROR_WRITING_COM_PORT){
    return  BT_ERROR_WRITING_COM_PORT_TEXT;
  }
  if(error_code ==  BT_ERROR_READING_COM_PORT){
    return  BT_ERROR_READING_COM_PORT_TEXT;
  }
  if(error_code ==  BT_NO_REPLY){
    return  BT_NO_REPLY_TEXT;
  }
  if(error_code ==  BT_UNABLE_TO_CONNECT ){
    return  BT_UNABLE_TO_CONNECT_TEXT;
  }
  if(error_code ==  BT_NOT_ENOUGH_BYTES){
    return  BT_NOT_ENOUGH_BYTES_TEXT;
  }
  if(error_code ==  NXT_PENDING_COMMUNICATION){
    return  NXT_PENDING_COMMUNICATION_TEXT;
  }
  if(error_code ==  NXT_MAILBOX_QUEUE_EMPTY){
    return  NXT_MAILBOX_QUEUE_EMPTY_TEXT;
  }
  if(error_code ==  NXT_REQUEST_FAILED){
    return  NXT_REQUEST_FAILED_TEXT;
  }
  if(error_code ==  NXT_UNKNOW_COMMAND){
    return  NXT_UNKNOW_COMMAND_TEXT;
  }
  if(error_code ==  NXT_INSANE_PACKET){
    return  NXT_INSANE_PACKET_TEXT;
  }
  if(error_code ==  NXT_DATA_OUT_OF_RANGE){
    return  NXT_DATA_OUT_OF_RANGE_TEXT;
  }
  if(error_code ==  NXT_COMMUNICATION_BUS_ERROR){
    return  NXT_COMMUNICATION_BUS_ERROR_TEXT;
  }
  if(error_code ==  NXT_BUFFER_FULL){
    return  NXT_BUFFER_FULL_TEXT;
  }
  if(error_code ==  NXT_INVALID_CHANNEL){
    return  NXT_INVALID_CHANNEL_TEXT;
  }
  if(error_code ==  NXT_CHANNEL_BUSY){
    return  NXT_CHANNEL_BUSY_TEXT;
  }
  if(error_code ==  NXT_NO_ACTIVE_PROGRAM){
    return  NXT_NO_ACTIVE_PROGRAM_TEXT;
  }
  if(error_code ==  NXT_ILLEGAL_SIZE){
    return  NXT_ILLEGAL_SIZE_TEXT;
  }
  if(error_code ==  NXT_INVALID_MAILBOX_QUEUE){
    return  NXT_INVALID_MAILBOX_QUEUE_TEXT;
  }
  if(error_code ==  NXT_INVALID_FIELD){
    return  NXT_INVALID_FIELD_TEXT;
  }
  if(error_code ==  NXT_BAD_INPUT_OUTPUT){
    return  NXT_BAD_INPUT_OUTPUT_TEXT;
  }
  if(error_code ==  NXT_OUT_OF_MEMORY){
    return  NXT_OUT_OF_MEMORY_TEXT;
  }
  if(error_code ==  NXT_BAD_ARGUMENTS){
    return  NXT_BAD_ARGUMENTS_TEXT;
  }
  if(error_code ==  NXT_I2C_TIMEOUT){
    return  NXT_I2C_TIMEOUT_TEXT;
  }
  if(error_code ==  NXT_NO_MORE_HANDLES){
    return  NXT_NO_MORE_HANDLES_TEXT;
  }
  if(error_code ==  NXT_NO_SPACE){
    return  NXT_NO_SPACE_TEXT;
  }
  if(error_code ==  NXT_NO_MORE_FILES){
    return  NXT_NO_MORE_FILES_TEXT;
  }
  if(error_code ==  NXT_END_OF_FILE_EXPECTED){
    return  NXT_END_OF_FILE_EXPECTED_TEXT;
  }
  if(error_code ==  NXT_END_OF_FILE){
    return  NXT_END_OF_FILE_TEXT;
  }
  if(error_code ==  NXT_NOT_A_LINEAR_FILE){
    return  NXT_NOT_A_LINEAR_FILE_TEXT;
  }
  if(error_code ==  NXT_FILE_NOT_FOUND){
    return  NXT_FILE_NOT_FOUND_TEXT;
  }
  if(error_code ==  NXT_HANDLE_ALREADY_CLOSED){
    return  NXT_HANDLE_ALREADY_CLOSED_TEXT;
  }
  if(error_code ==  NXT_NO_LINEAR_SPACE){
    return  NXT_NO_LINEAR_SPACE_TEXT;
  }
  if(error_code ==  NXT_UNDEFINED_FILE_ERROR){
    return  NXT_UNDEFINED_FILE_ERROR_TEXT;
  }
  if(error_code ==  NXT_FILE_BUSY){
    return  NXT_FILE_BUSY_TEXT;
  }
  if(error_code ==  NXT_NO_WRITE_BUFFERS){
    return  NXT_NO_WRITE_BUFFERS_TEXT;
  }
  if(error_code ==  NXT_APPEND_NOT_POSSIBLE){
    return  NXT_APPEND_NOT_POSSIBLE_TEXT;
  }
  if(error_code ==  NXT_FILE_IS_FULL){
    return  NXT_FILE_IS_FULL_TEXT;
  }
  if(error_code ==  NXT_FILE_EXISTS){
    return  NXT_FILE_EXISTS_TEXT;
  }
  if(error_code ==  NXT_MODULE_NOT_FOUND){
    return  NXT_MODULE_NOT_FOUND_TEXT;
  }
  if(error_code ==  NXT_OUT_OF_BOUNDARY){
    return  NXT_OUT_OF_BOUNDARY_TEXT;
  }
  if(error_code ==  NXT_ILLEGAL_FILE_NAME){
    return  NXT_ILLEGAL_FILE_NAME_TEXT;
  }
  if(error_code ==  NXT_ILLEGAL_HANDLE){
    return  NXT_ILLEGAL_HANDLE_TEXT;
  }
  if(error_code ==  PC_FILE_IO_ERROR){
    return  PC_FILE_IO_ERROR_TEXT;
  }
  //default
  return "Unknown error";
}

