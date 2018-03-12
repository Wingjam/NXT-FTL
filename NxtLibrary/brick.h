#ifndef  BRICK
#define  BRICK
#include <string>
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

using namespace std;

/**
  * Struct used to retriece device info
  * @see Brick#get_device_info
 */
struct NXTLIBRARY_API Device_info_t{
  /**
  * Brick name (maximum 15 characters)
  */
  string name;

  /**
  * Bluetooth address on the form XX:XX:XX:XX:XX:XX:XX
  */
  string bt_address;

  /**
  * free flash memory in bytes
  */
  unsigned int flash_memory;
};

/**
  * typedef for Device_info_t
  * @see Device_info_t
  * @see Brick#get_device_info
 */
typedef NXTLIBRARY_API Device_info_t Device_info;

/**
  * Struct uesd to retriece device version
  * @see Device_version_t
  * @see Brick#get_device_version
 */
struct NXTLIBRARY_API Device_version_t{

  /**
  * The firmware version currently running on the NXT
  */
  string firmware;

  /**
  * The protocol version currently running on the NXT
  */
  string protocol;
};

/**
  * typedef for Device_version_t
  * @see Brick#get_device_version
 */
typedef NXTLIBRARY_API Device_version_t Device_version;

using namespace std;

/**
  * Class to get/set brick name, get/set battery level, R/W to mailbox, start/stop programs, play sound, stop sound ect...
 */
class NXTLIBRARY_API Brick{
  public:
    /**
     * @param *connection [attach a connection]
     */
    Brick(Connection *connection);

    /**
     * Write a message to the NXT's mailbox
     * @param message [string to be send to the inbox (maximum 57 characters)]
     * @param inbox [specifies the inbox on the NXT(0-9)]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void write_msg(string message, int inbox, bool reply=false);

    /**
     * Read a message from the NXT's mailbox
     * @return returns a string from the inbox specified by inbox (maximum 57 characters)
     * @param inbox [specifies the inbox on the NXT(0-9)]
     * @param remove [true = remove the string from the inbox; false = leave the string in the inbox]
    */
    string read_msg(int inbox, bool remove);

    /**
     * Set the name of the NXT
     * @param name [new name of the NXT (maximum 15 characters)]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void set_name(string name, bool reply=false);

    /**
     * Keep the NXT alive
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void keep_alive(bool reply=false);

    /**
     * Make the NXT play a tone
     * @param freq [frequency in Hz (200-14000 Hz)]
     * @param time [duration in ms]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void play_tone(unsigned int freq, unsigned int time, bool reply=false);

    /**
     * Make the NXT beep
     * @param time [duration in ms]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void beep(unsigned int time, bool reply=false);

    /**
     * Play a soundfile from the NXT's flash
     * @param file [file name *.rso (sound file) (maximum 19 characters)]
     * @param loop [true = loop sound file indefinitely; false = play file once]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void play_soundfile(string file, bool loop, bool reply=false);

    /**
     * Stop all sound playback
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void stop_soundplayback(bool reply=false);

    /**
     * Start executing a program from the NXT's flash
     * @param file [file name *.rxe (downloaded program), *.rpg (On brick programs), *.rtm (Try me programs) (maximum 19 characters)]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void start_program(string file, bool reply=false);

    /**
     * Stop executing program
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void stop_programs(bool reply=false);

    /**
     * Get the current executing program
     * @return the current executing program (maximum 19 characters)
    */
    string get_current_program();

   /**
     * Get the name of the NXT
     * @return the brick name (maximum 15 characters)
    */
    string get_name();


    /**
     * Get the Bluetooth address of the NXT
     * @return the BT address on the form XX:XX:XX:XX:XX:XX:XX
    */
    string get_bt_address();

    /**
     * Get the firmware version currently running on the NXT
     * @return the firmware version on the form x.xx
    */
    string get_firmware_version();

    /**
     * Get the protocol version currently running on the NXT
     * @return the protocol version as on the form x.xxx
    */
    string get_protocol_version();

    /**
     * Get the battery level
     * @return the the battery level in mVolts
    */
    unsigned int get_battery_level();

    /**
     * Get free flash memory
     * @return the remaining free flash memory in bytes
    */
    unsigned int get_flash_memory();

    /**
     * Retrieve BT-address, brick name and free flash memory with a single call
     * @param info [used to store the return values]
    */
    void get_device_info(Device_info &info);

    /**
     * Retrieve Firmware version and Protrocol version with a single call
     * @param version [used to store the return values]
    */
    void get_device_version(Device_version &version);

  private:
    Connection *connection;
    Device_info info;
    Device_version version;
};
#endif
