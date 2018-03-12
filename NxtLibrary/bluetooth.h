#ifndef BLUETOOTH
#define BLUETOOTH
#include <windows.h>
#include <iostream>
#include <string>
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;
/**
  * Class for Bluetooth communication
*/
NXTLIBRARY_API class Bluetooth : public Connection{
  public:
    /**
     * Constructor for Bluetooth connection
     */
    NXTLIBRARY_API Bluetooth();
    NXTLIBRARY_API ~Bluetooth();
    /**
    * Send a byte string
    * @param *buffer [a pointer to a buffer that can hold the bytes to send]
    * @param num_bytes [the number of bytes to send]
    */
    NXTLIBRARY_API void send(unsigned char *buffer, unsigned int num_bytes);

    /**
    * Connect to the NXT
    * @param comport [specify the comport that is to used for the connection between the NXT and PC]
    */
    NXTLIBRARY_API void connect(unsigned int comport);

    /**
    * Disconnect from the NXT
    */
    NXTLIBRARY_API void disconnect();

    /**
    * Receive a byte string
    * @param *buffer [a pointer to a buffer that can hold the received bytes]
    * @param length [the number of bytes to receive]
    */
    NXTLIBRARY_API void receive(unsigned char *buffer, unsigned int length);

    /**
    * Flush the input and output buffer
    */
    NXTLIBRARY_API void flush();

    /**
    * Get the connection type
    * @return BT = bluetooth (see the CONNECTION_TYPE enumeration)
    */
    NXTLIBRARY_API Connection_type get_type();
    //unsigned int get_port();
  protected:
    /**
    * The comport handle
    */
    HANDLE handle;

    /**
    * Byte pointer used to send/recieve data
    */
    BYTE *byte;

    /**
    * nunmber of bytes recieved/send
    */
    DWORD number_bytes;

    /**
    * structure containing the time-out parameters for the comport
    */
    COMMTIMEOUTS timeout;

    /**
    * structure that defines the control setting for the comport
    */
    DCB dcb;
    unsigned int comport;
};
#endif
