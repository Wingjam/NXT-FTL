#ifndef COMMUNICATION
#define COMMUNICATION

#include <string>
#include <array>
#include "ColorSensorDto.h"
#include "TouchSensorDto.h"
#include "SensorDto.h"
#include "DistanceSensorDto.h"
#include "MotorDto.h"

class communication
{
public:
	communication();
	~communication();

	enum Sensor_type {
		TOUCH = 0,
		COLOR = 1,
		DISTANCE = 2
	};
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
		BLUETOOTH = 0,
		USB = 1
	};

	bool connect(Connection_type type);
	bool disconnect();
	bool initializeSensor(Sensor_port port, Sensor_type type);
	int getSensorValue(SensorDto sensorDto);
	std::array<int, 3> getSensorRGB(SensorDto sensorDto);
	std::string printSensorValue(SensorDto sensorDto);
	bool initializeMotor(MotorDto motorDto);
	long int getMotorRotation(MotorDto motorDto);
	bool isMotorRunning(MotorDto motorDto);
	void startMotor(MotorDto motorDto, char speed, unsigned int degrees = 0, bool reply = false);
	void coastMotor(MotorDto motorDto, char speed, unsigned int degrees = 0, bool reply = false);
	void stopMotor(MotorDto motorDto, bool reply = false);
};

#endif