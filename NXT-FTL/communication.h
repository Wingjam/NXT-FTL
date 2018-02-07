#pragma once

#include <string>
#include <array>
#include <memory>
#include "MotorDto.h"
#include "SensorDto.h"
#include "ColorSensorDto.h"
#include "TouchSensorDto.h"
#include "DistanceSensorDto.h"
#include "../NxtLibrary/connection.h"

class communication
{
	Connection *connection;
public:
	communication();
	~communication();

	enum Sensor_port {
		IN_1 = 0,
		IN_2 = 1,
		IN_3 = 2,
		IN_4 = 3
	};
	enum Motor_port {
		OUT_A = 0,
		OUT_B = 1,
		OUT_C = 2
	};
	enum Connection_type {
		BLUETOOF = 0,
		USB = 1
	};

	bool connect(Connection_type type, unsigned int comport);
	bool disconnect();
	void initializeSensor(TouchSensorDto& touchSensorDto, Sensor_port port);
	void initializeSensor(ColorSensorDto& colorSensorDto, Sensor_port port);
	void initializeSensor(DistanceSensorDto& distanceSensorDto, Sensor_port port);
	void getSensorValue(TouchSensorDto& touchSensorDto);
	void getSensorValue(ColorSensorDto& colorSensorDto);
	void getSensorValue(DistanceSensorDto& distanceSensorDto);
	std::array<int, 3> getSensorRGB(ColorSensorDto colorSensorDto);
	MotorDto initializeMotor(Motor_port port);
	bool isMotorRunning(MotorDto motorDto);
	void startMotor(MotorDto motorDto, char speed, unsigned int degrees = 0, bool reply = false);
	void coastMotor(MotorDto motorDto, char speed, unsigned int degrees = 0, bool reply = false);
	void stopMotor(MotorDto motorDto, bool reply = false);
};
