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

void communication::initializeSensor(TouchSensorDto & touchSensorDto, Sensor_port port)
{
}

void communication::initializeSensor(ColorSensorDto& colorSensorDto, Sensor_port port)
{
}

void communication::initializeSensor(DistanceSensorDto& distanceSensorDto, Sensor_port port)
{
}

void communication::updateSensorValue(TouchSensorDto& touchSensorDto)
{
}

void communication::updateSensorValue(ColorSensorDto& colorSensorDto)
{
}

void communication::updateSensorValue(DistanceSensorDto& distanceSensorDto)
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
