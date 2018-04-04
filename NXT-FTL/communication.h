#pragma once

#include <string>
#include <array>
#include <memory>
#include <map>
#include "motor_dto.h"
#include "sensor_dto.h"
#include "color_sensor_dto.h"
#include "touch_sensor_dto.h"
#include "distance_sensor_dto.h"
#include "../NxtLibrary/nxt.h"
#include "color2.h"


namespace nxtftl
{
	class communication
	{
	private:
		Connection* connection;
		map<int, Touch*> touchSensors;
		map<int, Sonar*> distanceSensors;
		map<int, color2*> colorSensors;
		map<int, Motor*> motors;

		/// <summary>
		/// Prints an NXT Exception error
		/// </summary>
		/// <param name="e">Nxt_exception error</param>
		void printError(Nxt_exception& e);
	public:
		communication();
		~communication();

		enum SensorPort {
			IN_1 = 0,
			IN_2 = 1,
			IN_3 = 2,
			IN_4 = 3
		};
		enum MotorPort {
			OUT_A = 0,
			OUT_B = 1,
			OUT_C = 2
		};
		enum ConnectionType {
			BLUETOOF = 0,
			USB = 1
		};

		/// <summary>
		/// Initialize connection to NXT brick
		/// If bluetooth mode is selected, this method will try to find 
		/// the comport connected to the brick.
		/// USB connection mode is not yet implemented.
		/// </summary>
		/// <param name="type">Type of connection to the brick (USB/Bluetooth)</param>
		/// <returns>Connection success result</returns>
		bool connect(ConnectionType type);

		/// <summary>
		/// This method connects to the brick using bluetooth
		/// </summary>
		/// <param name="comport">Bluetooth comport for the NXT</param>
		/// <returns>Connection success result</returns>
		bool connectWithBluetooth(unsigned int comport);

		/// <summary>
		/// Stops all motors and disconnects from the NXT cleanly
		/// </summary>
		/// <returns>Operation success result</returns>
		bool disconnect();

		/// <summary>
		/// Initializes a touch sensor
		/// </summary>
		/// <param name="touchSensorDto">Reference to the touch sensor to initialize</param>
		/// <param name="port">Port of the sensor on the brick</param>
		void initializeSensor(touch_sensor_dto& touchSensorDto, SensorPort port);

		/// <summary>
		/// Initializes a color sensor
		/// </summary>
		/// <param name="colorSensorDto">Reference to the color sensor to initialize</param>
		/// <param name="port">Port of the sensor on the brick</param>
		void initializeSensor(color_sensor_dto& colorSensorDto, SensorPort port);

		/// <summary>
		/// Initializes a distance sensor
		/// </summary>
		/// <param name="distanceSensorDto">Reference to the distance sensor to initialize</param>
		/// <param name="port">Port of the sensor on the brick</param>
		void initializeSensor(distance_sensor_dto& distanceSensorDto, SensorPort port);

		/// <summary>
		/// Reads and updates the touch sensor value
		/// </summary>
		/// <param name="touchSensorDto">Sensor dto</param>
		void updateSensorValue(touch_sensor_dto& touchSensorDto);

		/// <summary>
		/// Reads and updates the color sensor value
		/// </summary>
		/// <param name="colorSensorDto">Sensor dto</param>
		void updateSensorValue(color_sensor_dto& colorSensorDto);

		/// <summary>
		/// Reads and updates the distance sensor value
		/// </summary>
		/// <param name="distanceSensorDto">Sensor dto</param>
		void updateSensorValue(distance_sensor_dto& distanceSensorDto);

		/// <summary>
		/// Initializes a motor
		/// </summary>
		/// <param name="port">Port of the motor on the brick</param>
		motor_dto initializeMotor(MotorPort port);

		/// <summary>
		/// Determines if a motor is currently running
		/// </summary>
		/// <param name="motorDto">Motor to verify</param>
		bool isMotorRunning(motor_dto motorDto);

		/// <summary>
		/// Starts a motor (on)
		/// </summary>
		/// <param name="motorDto">Motor to start</param>
		/// <param name="speed">Speed of the motor (0-100)</param>
		/// <param name="degrees">Degrees of rotation (0-360)</param>
		/// <param name="reply">Force NXT to reply (false by default)</param>
		void startMotor(motor_dto motorDto, char speed, unsigned int degrees = 0, bool reply = false);

		/// <summary>
		/// Coasts a motor (off) (motor will not brake)
		/// </summary>
		/// <param name="motorDto">Motor to coast</param>
		/// <param name="reply">Force NXT to reply (false by default)</param>
		void coastMotor(motor_dto motorDto, bool reply = false);

		/// <summary>
		/// Stops a motor (brake)
		/// </summary>
		/// <param name="motorDto">Motor to stop</param>
		/// <param name="reply">Force NXT to reply (false by default)</param>
		void stopMotor(motor_dto motorDto, bool reply = false);

		/// <summary>
		/// Stops all initialized motors connected to the brick
		/// </summary>
		void stopAllMotors();

		/// <summary>
		/// Resets all color sensors connected to the brick
		/// </summary>
		void resetSensors();

		/// <summary>
		/// Returns the tacho count, which is essentially the motor angle in degree
		/// </summary>
		/// <param name="motorDto">Motor to monitor</param>
        void update_tacho_count(motor_dto& motorDto);
	private:

		/// <summary>
		/// Maps between the sensor dtos and the library objects
		/// </summary>
		/// <param name="port">Port of the sensor on the brick</param>
		Sensor_port mapSensorPort(SensorPort port);

		/// <summary>
		/// Maps between the motor dtos and the library objects
		/// </summary>
		/// <param name="port">Port of the motor on the brick</param>
		Motor_port mapMotorPort(MotorPort port);
	};
}
