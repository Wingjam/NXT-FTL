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


namespace nxtftl
{
	class communication
	{
	private:
		Connection* connection;
		map<int, Touch*> touchSensors;
		map<int, Sonar*> distanceSensors;
		map<int, Light*> colorSensors;
		map<int, Motor*> motors;

		void printError(Nxt_exception& e);
		void stopAllMotors();
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

		bool connect(ConnectionType type, unsigned int comport);
		bool disconnect();
		void initializeSensor(TouchSensorDto& touchSensorDto, SensorPort port);
		void initializeSensor(ColorSensorDto& colorSensorDto, SensorPort port);
		void initializeSensor(DistanceSensorDto& distanceSensorDto, SensorPort port);
		void updateSensorValue(TouchSensorDto& touchSensorDto);
		void updateSensorValue(ColorSensorDto& colorSensorDto);
		void updateSensorValue(DistanceSensorDto& distanceSensorDto);
		MotorDto initializeMotor(MotorPort port);
		bool isMotorRunning(MotorDto motorDto);
		void startMotor(MotorDto motorDto, char speed, unsigned int degrees = 0, bool reply = false);
		void coastMotor(MotorDto motorDto, bool reply = false);
		void stopMotor(MotorDto motorDto, bool reply = false);
	private:
		Sensor_port mapSensorPort(SensorPort port);
		Motor_port mapMotorPort(MotorPort port);
	};
}
