#ifndef CONNECTIONH
#define CONNECTIONH
#include <string>
#define NXT_BUFFER_SIZE 64

using namespace std;

/**
  * Connection type enumeration
  @see Connection#get_type
*/
enum Connection_type {
  /**
  * Bluetooth connection
  */
  BT = 0x00,

  /**
  * Network connection
  */
  NXT_NETWORK = 0x01
};

/**
  * Network server mode enumeration
  * @see Server_settings_t
  * @see Nxt_network#Connect
*/
enum Server_mode{
  /**
  * The server will close down when BT communication with the NXT is lost
  */
  CLOSE_DOWN = 0,

  /**
  * The server will (try) reconnect to the NXT when BT communication is lost
  */
  RECONNECT = 1
};

/**
  * Struct used to retriece server settings when a network connection is used
  * @see Nxt_network#Connect
  * @see Server_mode
 */
struct Server_settings_t{
  /**
  * The mode of the server
  */
  Server_mode mode;

  /**
  * The timeout of the client in milliseconds. If clients are
  * inactive for more than the timeout they will be thrown off. If timeout is zero timeout has been disabled
  */
  unsigned int  timeout;
};

/**
  * typedef for Server_settings_t
  * @see Server_settings_t
  * @see Nxt_network#Connect
 */
typedef Server_settings_t Server_settings;

/**
  * Abstract class for connections
*/
class Connection {
  public:
    virtual ~Connection(){};

    /**
    * Send a byte string
    * (must be implemented in sub class)
    * @param *buffer [a pointer to a buffer that can hold the bytes to send]
    * @param num_bytes [the number of bytes to send]
    */
    virtual void send(unsigned char *buffer,unsigned int num_bytes)=0;

    /**
    * Connect to the NXT using BT
    * (Dummy method does nothing - is implemented in sub class)
    * @param comport [specify the comport that is to used for the BT connection between the NXT and PC]
    */
    virtual void connect(unsigned int comport){return;}

    /**
    * Connect to the NXT using a network connection
    * (Dummy method does nothing - is implemented in sub class)
    * @param port [specify the port that is to used for the network connection between the NXT and PC]
    * @param ip_add [specify the IP-address]
    * @param settings [used to retrive server settings]
    * @param password [set the password]
    * @see Server_settings_t
    */
    virtual void connect(unsigned int port, string ip_add){return;}

    /**
    * Disconnect from the NXT
    * (must be implemented in sub class)
    */
    virtual void disconnect()=0;

    /**
    * Receive a byte string
    * (must be implemented in sub class)
    * @param *buffer [a pointer to a buffer that can hold the received bytes]
    * @param length [the number of bytes to receive]
    */
    virtual void receive(unsigned char *buffer, unsigned int length)=0;

    /**
    * Flush the input and output buffer
    * (must be implemented in sub class)
    */
    virtual void flush()=0;

    /**
    * Get the connection type
    * (must be implemented in sub class)
    * @return the connection type
    */
    virtual Connection_type get_type()=0;
  protected:
};
#endif
