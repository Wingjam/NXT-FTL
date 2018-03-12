#ifndef NXT_ERRORH
#define NXT_ERRORH
#include <exception>
#include <iostream>
#include <string>
#include <sstream>

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;
/**
* Error type enumeration
* @see Nxt_exception#error_type
*/
enum NXTLIBRARY_API Error_type{

  /**
  * Unknow error
  */
  UNKNOWN_ERROR = 0x00,

  /**
  * Network error
  */
  NETWORK_ERROR = 0x01,

  /**
  * NXT error
  */
  NXT_ERROR = 0x02,

  /**
  * Bluetooth error
  */
  BT_ERROR = 0x03,

  /**
  * PC file error
  */
  PC_FILE_ERROR = 0x04,

  /**
  * Server error
  */
  SERVER_ERROR = 0x05
};

/**
* Class used to throw exceptions when communication fails or the NXT reports an error
* @see Nxt_errors
*/
class NXTLIBRARY_API Nxt_exception: public exception
{
  public:
    /**
     * @param function_name [the name of the function that throws the exception]
     * @param class_name [the name of the Class that throws the exception]
     * @param error_code [the error code that is thrown]
     * @param description [a description of the error - if not speficied default description is used]
     */
    Nxt_exception(string function_name,string class_name, unsigned int error_code, string description="");
    ~Nxt_exception() throw();

    /**
     * What is the error
     * @return a string that describes the error
     */
    const char* what() const throw();

    /**
     * Get the error code that was thrown
     * @return the error code
     * @see Nxt_errors
     */
    unsigned int error_code();

    /**
     * Get the error type that was thrown
     * @return the error type
     * @see Error_type
     */
    Error_type error_type();


    /**
     * Who threw the exception
     * @return a string that tells the class and the function that threw the exception (used for debug)
     */
    const string who();
  protected:
    string function_name;
    string class_name;
    string description;
    unsigned int this_error_code;
    const char* error_info(unsigned int error_code) const;
};

#define SERVER_ERROR_LOW 0x0001
#define SERVER_ERROR_HIGH 0x001f

#define NXT_ERROR_LOW 0x0020
#define NXT_ERROR_HIGH 0x00FF

#define BT_ERROR_LOW 0x0100
#define BT_ERROR_HIGH 0x014F

#define PC_FILE_ERROR_LOW 0x0150
#define PC_FILE_ERROR_HIGH 0x015F

#define NETWORK_ERROR_LOW 0x0160
#define NETWORK_ERROR_HIGH 0x016f

/**
* Error code enumeration (error code list)
* @see
* <UL>
* <LI>0x0001 to 0x001F are reservede for server errors
* <LI>0x0020 to 0x00FF are reserved for NXT brick errors
* <LI>0x0100 to 0x014F are reserved for Bluetooth errors
* <LI>0x0150 to 0x015F are reserved for PC-file errors
* <LI>0x0160 to 0x016F are reservede for network errors
* </UL>
*/
enum NXTLIBRARY_API Nxt_errors{
  //Network erros

  /**
  * Network error: Network communication error
  */
  NETWORK_COM_ERROR = 0x0160,

  /**
   *Server error: Server is full - connection was rejected
  */
  SERVER_FULL = 0x0002,

  /**
   *Server error: Server lost connection with NXT
  */
  SERVER_LOST_NXT = 0x0003,

  /**
   *Server error: Server encountered an Bluetooth communication error
  */
  SERVER_BT_ERROR = 0x0004,

  /**
   *Server error: Bluetooth link on server is bussy
  */
  SERVER_BT_BUSSY = 0x0005,

  /**
   *Server error: Server didn't recieved enough bytes from the NXT
  */
  SERVER_BT_NOT_ENOUGH_BYTES = 0x0006,

  /**
   *Server error: Server rejected password
  */
  SERVER_PASSWORD_FAIL = 0x0007,

  /**
   *Server error: Server rejected IP address
  */
  SERVER_BANNED = 0x0008,

  //Bluetooth errors
  /**
   *Bluetooth error: Invalid COM port specified
   */
  BT_INVALID_COM_PORT = 0x0101,

  /**
   *Bluetooth error: Error sending
   */
  BT_ERROR_WRITING_COM_PORT = 0x0102,

  /**
   *Bluetooth error: Error receiving
   */
  BT_ERROR_READING_COM_PORT = 0x0103,

  /**
   *Bluetooth error: No reply from the NXT
   */
  BT_NO_REPLY = 0x0104,

  /**
   *Bluetooth error: Didn't recieved enough bytes from the NXT
   */
  BT_NOT_ENOUGH_BYTES = 0x0105,

  /**
   *Bluetooth error: Unsable to connect
   */
  BT_UNABLE_TO_CONNECT = 0x0106,

  //PC error
  /**
   *PC-file error: Unable to read or write to file PC
   */
  PC_FILE_IO_ERROR = 0x0150,

  //LEGO BRICk

  /**
   *NXT error: Bytes/answer from I2C sensor not ready which resulted in a timeout
   */
  NXT_I2C_TIMEOUT = 0x0021,

  /**
   *No more handles
   */
  NXT_NO_MORE_HANDLES = 0x0081,

  /**
   *NXT error: No more space in flash
   */
  NXT_NO_SPACE = 0x0082,

  /**
   *NXT error: No more files can be created
  */
  NXT_NO_MORE_FILES = 0x0083,

  /**
   *NXT error: EOF expected
  */
  NXT_END_OF_FILE_EXPECTED = 0x0084,

  /**
   *NXT error: EOF
  */
  NXT_END_OF_FILE = 0x0085,

  /**
   *NXT error: File specified is not linear
  */
  NXT_NOT_A_LINEAR_FILE = 0x0086,

  /**
   *NXT error: File not found
  */
  NXT_FILE_NOT_FOUND = 0x0087,

  /**
   *NXT error: File is already closed
  */
  NXT_HANDLE_ALREADY_CLOSED = 0x0088,

  /**
   *NXT error: No linear space
  */
  NXT_NO_LINEAR_SPACE = 0x0089,

  /**
   *NXT error: Undefined file error occurred
  */
  NXT_UNDEFINED_FILE_ERROR = 0x008A,

  /**
   *NXT error: File is busy
  */
  NXT_FILE_BUSY = 0x008B,

  /**
   *NXT error: No write buffers
  */
  NXT_NO_WRITE_BUFFERS = 0x008C,

  /**
   *NXT error: Unable to append
  */
  NXT_APPEND_NOT_POSSIBLE = 0x008D,

  /**
   *NXT error: File is full
  */
  NXT_FILE_IS_FULL = 0x008E,

  /**
   *NXT error: File already exists
  */
  NXT_FILE_EXISTS = 0x008F,

  /**
   *NXT error: Module not found
  */
  NXT_MODULE_NOT_FOUND = 0x0090,

  /**
   *NXT error: Out of boundary
  */
  NXT_OUT_OF_BOUNDARY = 0x0091,

  /**
   *NXT error: Filename specified is illegal
  */
  NXT_ILLEGAL_FILE_NAME = 0x0092,

  /**
   *NXT error: Filehandle specified is illegal
  */
  NXT_ILLEGAL_HANDLE = 0x0093,

  /**
   *NXT error: Pending communication transaction in progress
  */
  NXT_PENDING_COMMUNICATION = 0x0020,

  /**
   *NXT error: Specified mailbox queue is empty
  */
  NXT_MAILBOX_QUEUE_EMPTY = 0x0040,

  /**
   *NXT error: Request failed (i.e. specified file not found)
  */
  NXT_REQUEST_FAILED = 0x00BD,

  /**
   *NXT error: Unknown command opcode
  */
  NXT_UNKNOW_COMMAND = 0x00BE,

  /**
   *NXT error: Insane packet
  */
  NXT_INSANE_PACKET = 0x00BF,

  /**
   *NXT error: Data contains out-of-range value
  */
  NXT_DATA_OUT_OF_RANGE = 0x00C0,

  /**
   *NXT error: Communication bus error
  */
  NXT_COMMUNICATION_BUS_ERROR = 0x00DD,

  /**
   *NXT error: No free memory in communication buffer
  */
  NXT_BUFFER_FULL = 0x00DE,

  /**
   *NXT error: Specified channel/connection is not valid
  */
  NXT_INVALID_CHANNEL = 0x00DF,

  /**
   *NXT error: Specified channel/connection not configured or busy
  */
  NXT_CHANNEL_BUSY = 0x00E0,

  /**
   *NXT error: No active program is running
  */
  NXT_NO_ACTIVE_PROGRAM = 0x00EC,

  /**
   *NXT error: Illegal size specified
  */
  NXT_ILLEGAL_SIZE = 0x00ED,

  /**
   *NXT error: Illegal mailbox queue ID specified
  */
  NXT_INVALID_MAILBOX_QUEUE = 0x00EE,

  /**
   *NXT error: Attempted to access invalid field of a structure
  */
  NXT_INVALID_FIELD = 0x00EF,

  /**
   *NXT error: Bad input or output specified
  */
  NXT_BAD_INPUT_OUTPUT = 0x00F0,

  /**
   *NXT error: Insufficient memory available
  */
  NXT_OUT_OF_MEMORY = 0x00FB,

  /**
   *NXT error: Bad arguments
  */
  NXT_BAD_ARGUMENTS  = 0x00FF
};

/*************************************
NXT brick errors
*************************************/
//I2C error not a part of "LEGO" errors
#define NXT_I2C_TIMEOUT_TEXT "Bytes/answer from I2C sensor not ready which resulted in a timeout"

//added from appendix 1 in BT SDK (for files)
#define NXT_NO_MORE_HANDLES_TEXT "No more handles"
#define NXT_NO_SPACE_TEXT "No space"
#define NXT_NO_MORE_FILES_TEXT "No more files"
#define NXT_END_OF_FILE_EXPECTED_TEXT "End of file expected"
#define NXT_END_OF_FILE_TEXT "End of file"
#define NXT_NOT_A_LINEAR_FILE_TEXT "Not a linear file"
#define NXT_FILE_NOT_FOUND_TEXT "File not found"
#define NXT_HANDLE_ALREADY_CLOSED_TEXT "Handle already closed"
#define NXT_NO_LINEAR_SPACE_TEXT "No linear space"
#define NXT_UNDEFINED_FILE_ERROR_TEXT "Undefined file error"
#define NXT_FILE_BUSY_TEXT "File is busy"
#define NXT_NO_WRITE_BUFFERS_TEXT "No write buffers"
#define NXT_APPEND_NOT_POSSIBLE_TEXT "Append not possible"
#define NXT_FILE_IS_FULL_TEXT "File is full"
#define NXT_FILE_EXISTS_TEXT "File already exists"
#define NXT_MODULE_NOT_FOUND_TEXT "Module not found"
#define NXT_OUT_OF_BOUNDARY_TEXT "Out of boundary"
#define NXT_ILLEGAL_FILE_NAME_TEXT "llegal file name"
#define NXT_ILLEGAL_HANDLE_TEXT "Illegal handle"

//errors from the NXT (see the LEGO BT SDK appendix 2)
#define NXT_PENDING_COMMUNICATION_TEXT "Pending communication transaction in progress"
#define NXT_MAILBOX_QUEUE_EMPTY_TEXT "Specified mailbox queue is empty"
#define NXT_REQUEST_FAILED_TEXT "Request failed (i.e. specified file not found)"
#define NXT_UNKNOW_COMMAND_TEXT "Unknown command opcode"
#define NXT_INSANE_PACKET_TEXT "Insane packet"
#define NXT_DATA_OUT_OF_RANGE_TEXT "Data contains out-of-range values"
#define NXT_COMMUNICATION_BUS_ERROR_TEXT "Communication bus error"
#define NXT_BUFFER_FULL_TEXT "No free memory in communication buffer"
#define NXT_INVALID_CHANNEL_TEXT "Pecified channel/connection is not valid"
#define NXT_CHANNEL_BUSY_TEXT "Specified channel/connection not configured or busy"
#define NXT_NO_ACTIVE_PROGRAM_TEXT "No active program running"
#define NXT_ILLEGAL_SIZE_TEXT "Illegal size specified"
#define NXT_INVALID_MAILBOX_QUEUE_TEXT "Illegal mailbox queue ID specified"
#define NXT_INVALID_FIELD_TEXT "Attempted to access invalid field of a structure"
#define NXT_BAD_INPUT_OUTPUT_TEXT "Bad input or output specified"
#define NXT_OUT_OF_MEMORY_TEXT "Insufficient memory available"
#define NXT_BAD_ARGUMENTS_TEXT "Bad arguments"

/*************************************
Network errors
*************************************/
#define NETWORK_COM_ERROR_TEXT "Network communication occurred."

/*************************************
Server errors
*************************************/
#define SERVER_LOST_NXT_TEXT "Server lost communication with NXT"
#define SERVER_BT_NOT_ENOUGH_BYTES_TEXT "Server didn't recieved enough bytes from the NXT"
#define SERVER_BT_BUSSY_TEXT "Bluetooth link on server is bussy"
#define SERVER_BT_ERROR_TEXT "Server encountered an Bluetooth communication error"
#define SERVER_FULL_TEXT "Server is full - connection rejected"
#define SERVER_PASSWORD_FAIL_TEXT "Password was rejected"
#define SERVER_BANNED_TEXT "IP address rejected"

/*************************************
Bluetooth errors
*************************************/
#define BT_INVALID_COM_PORT_TEXT "Invalid COM port or COM port in use"
#define BT_ERROR_WRITING_COM_PORT_TEXT "Error sending"
#define BT_ERROR_READING_COM_PORT_TEXT "Error receiving"
#define BT_NO_REPLY_TEXT "No reply from the NXT"
#define BT_NOT_ENOUGH_BYTES_TEXT "Didn't recieved enough bytes from the NXT"
#define BT_UNABLE_TO_CONNECT_TEXT "Unsable to connect to the NXT"

/*************************************
PC-file errors
*************************************/
#define PC_FILE_IO_ERROR_TEXT "PC-file error: Unable to read or write from file"

#endif
