#include "communication.h"
#include <algorithm>

using namespace NXTFTL;

communication::communication()
{
}


communication::~communication()
{
	// Clean up memory (deletes)
}

bool communication::connect(ConnectionType type, unsigned int comport)
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
		stopAllMotors();
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

void communication::initializeSensor(TouchSensorDto & touchSensorDto, SensorPort port)
{
	Touch* touchSensor = new Touch(mapSensorPort(port), connection);
	touchSensorDto.port = port;
	touchSensors.emplace(port, touchSensor);
}

void communication::initializeSensor(ColorSensorDto& colorSensorDto, SensorPort port)
{
	Light* colorSensor = new Light(mapSensorPort(port), connection);
	colorSensorDto.port = port;
	colorSensors.emplace(port, colorSensor);
}

void communication::initializeSensor(DistanceSensorDto& distanceSensorDto, SensorPort port)
{
	Sonar* distanceSensor = new Sonar(mapSensorPort(port), connection);
	distanceSensorDto.port = port;
	distanceSensors.emplace(port, distanceSensor);
}

MotorDto communication::initializeMotor(MotorPort port)
{
	Motor* motor = new Motor(mapMotorPort(port), connection);
	motors.emplace(port, motor);
	return MotorDto{ static_cast<unsigned int>(port) };
}

Sensor_port communication::mapSensorPort(SensorPort port) 
{
	Sensor_port mappedPort;
	switch (port)
	{
		case SensorPort::IN_1:
			mappedPort = Sensor_port::IN_1;
			break;
		case SensorPort::IN_2:
			mappedPort = Sensor_port::IN_2;
			break;
		case SensorPort::IN_3:
			mappedPort = Sensor_port::IN_3;
			break;
		case SensorPort::IN_4:
			mappedPort = Sensor_port::IN_4;
			break;
	}
	return mappedPort;
}

Motor_port communication::mapMotorPort(MotorPort port)
{
	Motor_port mappedPort;
	switch (port)
	{
		case MotorPort::OUT_A:
			mappedPort = Motor_port::OUT_A;
			break;
		case MotorPort::OUT_B:
			mappedPort = Motor_port::OUT_B;
			break;
		case MotorPort::OUT_C:
			mappedPort = Motor_port::OUT_C;
			break;
	}
	return mappedPort;
}

void communication::updateSensorValue(TouchSensorDto& touchSensorDto)
{
	Touch* touchSensor = touchSensors[touchSensorDto.port];
	touchSensorDto.isPressed = touchSensor->read();
}

void communication::updateSensorValue(color_sensor_dto& colorSensorDto)
{
	Light* colorSensor = colorSensors[colorSensorDto.port];
	Rgb_color result{};
	string value = colorSensor->print();
	colorSensorDto.BlueValue = result.blue;
	colorSensorDto.GreenValue = result.green;
	colorSensorDto.RedValue = result.red;
}

void communication::updateSensorValue(distance_sensor_dto& distanceSensorDto)
{
	Sonar* distanceSensor = distanceSensors[distanceSensorDto.port];
	int value = distanceSensor->read();
	distanceSensorDto.Distance = value;
}

void communication::stopAllMotors() {
	for_each(motors.begin(), motors.end(), [](pair<int, Motor*> pair) { pair.second->stop(); });
}

bool communication::isMotorRunning(motor_dto motorDto)
{
	Motor* motor = motors[motorDto.port];
	return motor->is_running();
}

void communication::startMotor(MotorDto motorDto, char speed, unsigned int degrees /* = 0 */, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->on(speed, degrees, reply);
}

void communication::coastMotor(MotorDto motorDto, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->off(reply);
}

void communication::stopMotor(MotorDto motorDto, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->stop(reply);
}
