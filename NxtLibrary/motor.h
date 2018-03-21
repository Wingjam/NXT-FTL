#ifndef MOTOR
#define MOTOR
#include <iostream>
#include <string>
#include "connection.h"

/**
* Motor port enumeration
* @see Motor
*/
enum Motor_port {

  /**
  * OUT A
  */
  OUT_A = 0,

  /**
  * OUT B
  */
  OUT_B = 1,

  /**
  * OUT C
  */
  OUT_C = 2
};


//should be placed in some enumeration
//Motor mode
#define MOTOR_ON 0x01
#define BREAK 0x02
#define REGULATED 0x04

//Motor regulation
#define REG_IDLE 0x00
#define MOTOR_SPEED 0x01
#define MOTOR_SYNC 0x02

//Motor run state
#define RUN_IDLE 0x00
#define RAMP_UP 0x10
#define RUNNING 0x20
#define RAMP_DOWN 040

#define DEFAULT_TURN_RATIO 100

//will be implemented later
/*typedef struct states{
  unsigned char mode;
  unsigned char reg_mode;
  unsigned char run_state;
  char turnratio;
  long unsigned int tacho_limit;
  long int tacho_count;
  long int block_count;
  long int rotation_count;
}motor_state;

typedef struct set_state{
  unsigned char mode;
  unsigned char reg_mode;
  unsigned char run_state;
  char turnratio;
  unsigned int set_tacho_limit;
  char set_speed;
}output_state;*/

using namespace std;
/**
  * Class for the NXT motors
 */
class Motor{
  public:
    /**
     * @param port [which motor port to use]
     * @param *connection [attach a connection]
     */
    Motor(Motor_port port, Connection *connection);

    /**
    * Make the motor turn
    * @param speed [set the speed of the motor (the sign will determine the direction of the motor)]
    * @param degree [makes the motor turn x degrees (if set to zero the motor will turn forever)]
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void on(char speed, unsigned int degrees=0, bool reply=false);

    /**
    * Is the motor running
    * @return true if the motor is running otherwise false
    */
    bool is_running();

    /**
    * Brings the motor to a full stop
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void stop(bool reply=false);

    /**
    * Make the motor coast
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void off(bool reply=false);

    /**
    * Reset the position of the motor
    * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void reset_rotation(bool reply=false);

    /**
    * Get the position of the motor
    * @return the position of the motor
    */
    long int get_rotation();

    /**
    * Move to a target degree  - function will return when the motor is done moving (this is experimental)
    * @param speed [set the speed of the motor (the sign has no influence)]
    * @param target [set the target degree]
    * @param tolerance [allow some torance to avoid hanging the function (don't set this below 4)]
    */
    void move_to(char speed, long int target, int tolerance);

    /**
     * Get the motor port
     * @return the motor port
    */
    Motor_port get_port();
	void get_output_state();
	long int tacho_count;
  private:
    Motor_port port;
    Connection *connection;
    void move_to(char speed, long int target);
    unsigned char mode;
    unsigned char reg_mode;
    unsigned char run_state;
    int turnratio;
    long unsigned int tacho_limit;
    long int block_count;
    long int rotation_count;
    void reset(bool relative, bool reply=false);
    void set_output_state(char set_speed, unsigned char set_mode,unsigned char set_regulation,
                          char set_turn_ratio, unsigned char set_run_state,
                          unsigned int set_tacho_limit, bool reply=false);
    /*Test function*/
    //int Motor::slow_down();
};
#endif

