#include "communication.h"
#include "../NxtLibrary/nxt.h"

communication::communication()
{
}


communication::~communication()
{
}

bool communication::connect(Connection_type type, unsigned int comport)
{
	bool success = false;
	switch (type)
	{
		case communication::BLUETOOF:
			connection = new Bluetooth();
			connection->connect(comport);
			success = true;
			break;
		case communication::USB:
			break;
	}
	return success;
}

bool communication::disconnect()
{
	bool success = false;
	connection->disconnect();
	return success;
}

void communication::initializeSensor(TouchSensorDto & touchSensorDto, Sensor_port port)
{
}

void communication::initializeSensor(ColorSensorDto& colorSensorDto, Sensor_port port)
{
}

void communication::initializeSensor(DistanceSensorDto& distanceSensorDto, Sensor_port port)
{
}

void communication::getSensorValue(TouchSensorDto& touchSensorDto)
{
}

void communication::getSensorValue(ColorSensorDto& colorSensorDto)
{
}

void communication::getSensorValue(DistanceSensorDto& distanceSensorDto)
{
}

MotorDto communication::initializeMotor(Motor_port port)
{
	return MotorDto{};
}

bool communication::isMotorRunning(MotorDto motorDto)
{
	return false;
}

void communication::startMotor(MotorDto motorDto, char speed, unsigned int degrees, bool reply)
{
}

void communication::coastMotor(MotorDto motorDto, char speed, unsigned int degrees, bool reply)
{
}

void communication::stopMotor(MotorDto motorDto, bool reply)
{
}
