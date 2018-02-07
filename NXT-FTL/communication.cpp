#include "communication.h"
#include "../NxtLibrary/nxt.h"

Connection *connection;

communication::communication()
{
}


communication::~communication()
{
}

bool communication::connect(Connection_type type)
{
	switch (Connection_type)
	{
		case communication::BLUETOOTH:
			break;
		case communication::USB:
			break;
		default:
			break;
	}
}

bool communication::disconnect()
{
	return false;
}

bool communication::initializeSensor(Sensor_port port, Sensor_type type)
{
	return false;
}

int communication::getSensorValue(SensorDto sensorDto)
{
	return 0;
}

std::array<int, 3> communication::getSensorRGB(Sensor_port port)
{
	return std::array<int, 3>();
}

std::string communication::printSensorValue(Sensor_port port)
{
	return std::string();
}

bool communication::initializeMotor(Motor_port port)
{
	return false;
}

long int communication::getMotorRotation(Motor_port port)
{
	return 0;
}

bool communication::isMotorRunning(Motor_port port)
{
	return false;
}

void communication::startMotor(Motor_port port, char speed, unsigned int degrees, bool reply)
{
}

void communication::coastMotor(Motor_port port, char speed, unsigned int degrees, bool reply)
{
}

void communication::stopMotor(Motor_port port, bool reply)
{
}
