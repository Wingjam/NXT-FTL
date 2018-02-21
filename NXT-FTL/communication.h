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

		bool connect(ConnectionType type);
		bool connectWithBluetooth(unsigned int comport);
		bool disconnect();
		void initializeSensor(touch_sensor_dto& touchSensorDto, SensorPort port);
		void initializeSensor(color_sensor_dto& colorSensorDto, SensorPort port);
		void initializeSensor(distance_sensor_dto& distanceSensorDto, SensorPort port);
		void updateSensorValue(touch_sensor_dto& touchSensorDto);
		void updateSensorValue(color_sensor_dto& colorSensorDto);
		void updateSensorValue(distance_sensor_dto& distanceSensorDto);
		motor_dto initializeMotor(MotorPort port);
		bool isMotorRunning(motor_dto motorDto);
		void startMotor(motor_dto motorDto, char speed, unsigned int degrees = 0, bool reply = false);
		void coastMotor(motor_dto motorDto, bool reply = false);
		void stopMotor(motor_dto motorDto, bool reply = false);
		void stopAllMotors();
		void resetSensors();
	private:
		Sensor_port mapSensorPort(SensorPort port);
		Motor_port mapMotorPort(MotorPort port);
	};
}
