#include "communication.h"
#include "../NxtLibrary/nxt.h"
using namespace nxtftl;

communication::communication()
{
}


communication::~communication()
{
}

bool communication::connect(Connection_type type, unsigned int comport)
{
	switch (type)
	{
		case communication::BLUETOOF:
			try
			{
				connection = new Bluetooth();
				connection->connect(comport);
				return true;
			}
			catch (Nxt_exception& e) {
				printError(e);
				connection->disconnect();
			}
			break;
		case communication::USB:
			return false;
	}
}

bool communication::disconnect()
{
	bool success = false;
	try
	{
		connection->disconnect();
	}
	catch (Nxt_exception& e) {
		printError(e);
		connection->disconnect();
	}
	return success;
}

void communication::printError(Nxt_exception& e)
{
	cout << e.what() << endl;
	cout << "error code: " << e.error_code() << endl;
	cout << "error type: " << e.error_type() << endl;
	cout << e.who() << endl;
}

void communication::initializeSensor(touch_sensor_dto & touchSensorDto, Sensor_port port)
{
}

void communication::initializeSensor(color_sensor_dto& colorSensorDto, Sensor_port port)
{
}

void communication::initializeSensor(distance_sensor_dto& distanceSensorDto, Sensor_port port)
{
}

void communication::updateSensorValue(touch_sensor_dto& touchSensorDto)
{
}

void communication::updateSensorValue(color_sensor_dto& colorSensorDto)
{
}

void communication::updateSensorValue(distance_sensor_dto& distanceSensorDto)
{
}

motor_dto communication::initializeMotor(Motor_port port)
{
	return motor_dto{};
}

bool communication::isMotorRunning(motor_dto motorDto)
{
	return false;
}

void communication::startMotor(motor_dto motorDto, char speed, unsigned int degrees, bool reply)
{
}

void communication::coastMotor(motor_dto motorDto, char speed, unsigned int degrees, bool reply)
{
}

void communication::stopMotor(motor_dto motorDto, bool reply)
{
}
