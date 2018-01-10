#ifndef NETWORK_H
#define NETWORK_H
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "connection.h"

using namespace std;


/**
  * Class for Network communication
*/

class Nxt_network : public Connection{
  public:
    /**
     * Constructor for Network connection
     */
    Nxt_network();
    ~Nxt_network();
    /**
    * Send a byte string
    * @param *buffer [a pointer to a buffer that can hold the bytes to send]
    * @param num_bytes [the number of bytes to send]
    */
    void send(unsigned char *buffer, unsigned int num_bytes);

    /**
    * Connect to the NXT using a network connection
    * @param port [specify the port that is to used for the network connection between the NXT and PC]
    * @param ip_add [specify the IP-address]
    * @param settings [used to retrive server settings]
    * @param password [set the password]
    * @see Server_settings_t
    */
    void connect(unsigned int port, string ip_add);

    /**
    * Disconnect from the NXT
    */
    void disconnect();

    /**
    * Receive a byte string
    * @param *buffer [a pointer to a buffer that can hold the received bytes]
    * @param length [the number of bytes to receive]
    */
    void receive(unsigned char *buffer, unsigned int length);

    /**
    * Flush the input and output buffer
    */
    void flush();

    /**
    * Get the connection type
    * @return  NXT_NETWORK (see the CONNECTION_TYPE enumeration)
    */
    Connection_type get_type();
    //unsigned int get_port();
  private:
    unsigned char *buffer;
    WSAData ws_data;
    WORD ws_ver;
    int ws_status;
    SOCKET my_sock;
    int test(sockaddr_in &temp);
    string error_to_string(int err);
};
#endif
