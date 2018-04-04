#include "communication.h"
#include <algorithm>

using namespace nxtftl;

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
communication::communication()
{
}

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
communication::~communication()
{
}

bool communication::connect(ConnectionType type)
{
	bool connected = false;
	unsigned int comport = 0;

	switch (type)
	{
		case communication::BLUETOOF:
			while (comport <= 40 && !connected)
			{
				try
				{
					connection = new Bluetooth();
					connection->connect(comport);
					connected = true;
					cout << "NXT connected on comport " << comport << endl;
				}
				catch (Nxt_exception) {
					cout << "Could not connect to NXT on comport " << comport << endl;
					connection->disconnect();
					++comport;
				}
			}
			break;
		case communication::USB:
			connected = false;
			throw "Not implemented yet.";
	}
}

bool communication::connectWithBluetooth(unsigned int comport) 
{
	try
	{
		connection = new Bluetooth();
		connection->connect(comport);
		return true;
	}
	catch (Nxt_exception) {
		cout << "Could not connect to NXT on comport " << comport << endl;
		connection->disconnect();
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
		cout << "NXT disconnected successfully." << endl;
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

void communication::initializeSensor(touch_sensor_dto & touchSensorDto, SensorPort port)
{
	Touch* touchSensor = new Touch(mapSensorPort(port), connection);
	touchSensorDto.port = port;
	touchSensors.emplace(port, touchSensor);
}

void communication::initializeSensor(color_sensor_dto& colorSensorDto, SensorPort port)
{
	color2* colorSensor = new color2(mapSensorPort(port), connection);
	colorSensorDto.port = port;
	colorSensors.emplace(port, colorSensor);
}

void communication::initializeSensor(distance_sensor_dto& distanceSensorDto, SensorPort port)
{
	Sonar* distanceSensor = new Sonar(mapSensorPort(port), connection);
	distanceSensorDto.port = port;
	distanceSensors.emplace(port, distanceSensor);
}

motor_dto communication::initializeMotor(MotorPort port)
{
	Motor* motor = new Motor(mapMotorPort(port), connection);
	motors.emplace(port, motor);
	return motor_dto{ static_cast<unsigned int>(port) };
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

void communication::updateSensorValue(touch_sensor_dto& touchSensorDto)
{
	Touch* touchSensor = touchSensors[touchSensorDto.port];
	touchSensorDto.is_pressed = touchSensor->read();
}

void communication::updateSensorValue(color_sensor_dto& colorSensorDto)
{
	color2* colorSensor = colorSensors[colorSensorDto.port];
	colorSensorDto.intensity = colorSensor->getValue();
}

void communication::updateSensorValue(distance_sensor_dto& distanceSensorDto)
{
	Sonar* distanceSensor = distanceSensors[distanceSensorDto.port];
	int value = distanceSensor->read();
	distanceSensorDto.distance = value;
}

void communication::stopAllMotors() {
	for_each(motors.begin(), motors.end(), [](pair<int, Motor*> pair) { pair.second->stop(); });
}

void communication::resetSensors() {
	//for_each(colorSensors.begin(), colorSensors.end(), [](pair<int, Sensor*> pair) { pair.second->reset(); });
}

bool communication::isMotorRunning(motor_dto motorDto)
{
	Motor* motor = motors[motorDto.port];
	return motor->is_running();
}

void communication::startMotor(motor_dto motorDto, char speed, unsigned int degrees /* = 0 */, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->on(speed, degrees, reply);
}

void communication::coastMotor(motor_dto motorDto, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->off(reply);
}

void communication::stopMotor(motor_dto motorDto, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->stop(reply);
}

void communication::update_tacho_count(motor_dto& motorDto)
{
	Motor* motor = motors[motorDto.port];
	motor->get_output_state();
	motorDto.tacho_count = motor->tacho_count;
}
