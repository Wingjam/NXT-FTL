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

/// <summary>
/// Initialize connection to NXT brick
/// If bluetooth mode is selected, this method will try to find 
/// the comport connected to the brick.
/// USB connection mode is not yet implemented.
/// </summary>
/// <param name="type">Type of connection to the brick (USB/Bluetooth)</param>
/// <returns>Connection success result</returns>
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
				}
				catch (Nxt_exception& e) {
					cout << "Could not connect to NXT on comport " << comport << endl;
					connection->disconnect();
					++comport;
				}
			}
			cout << "NXT connected on comport " << comport << endl;
			break;
		case communication::USB:
			connected = false;
			throw "Not implemented yet.";
	}
}

/// <summary>
/// This method connects to the brick using bluetooth
/// </summary>
/// <param name="comport">Bluetooth comport for the NXT</param>
/// <returns>Connection success result</returns>
bool communication::connectWithBluetooth(unsigned int comport) 
{
	try
	{
		connection = new Bluetooth();
		connection->connect(comport);
		return true;
	}
	catch (Nxt_exception& e) {
		cout << "Could not connect to NXT on comport " << comport << endl;
		connection->disconnect();
		return false;
	}
}

/// <summary>
/// Stops all motors and disconnects from the NXT cleanly
/// </summary>
/// <returns>Operation success result</returns>
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

/// <summary>
/// Prints an NXT Exception error
/// </summary>
/// <param name="e">Nxt_exception error</param>
void communication::printError(Nxt_exception& e)
{
	cout << e.what() << endl;
	cout << "error code: " << e.error_code() << endl;
	cout << "error type: " << e.error_type() << endl;
	cout << e.who() << endl;
}

/// <summary>
/// Initializes a touch sensor
/// </summary>
/// <param name="touchSensorDto">Reference to the touch sensor to initialize</param>
/// <param name="port">Port of the sensor on the brick</param>
void communication::initializeSensor(touch_sensor_dto & touchSensorDto, SensorPort port)
{
	Touch* touchSensor = new Touch(mapSensorPort(port), connection);
	touchSensorDto.port = port;
	touchSensors.emplace(port, touchSensor);
}

/// <summary>
/// Initializes a color sensor
/// </summary>
/// <param name="colorSensorDto">Reference to the color sensor to initialize</param>
/// <param name="port">Port of the sensor on the brick</param>
void communication::initializeSensor(color_sensor_dto& colorSensorDto, SensorPort port)
{
	color2* colorSensor = new color2(mapSensorPort(port), connection);
	colorSensorDto.port = port;
	colorSensors.emplace(port, colorSensor);
}

/// <summary>
/// Initializes a distance sensor
/// </summary>
/// <param name="distanceSensorDto">Reference to the distance sensor to initialize</param>
/// <param name="port">Port of the sensor on the brick</param>
void communication::initializeSensor(distance_sensor_dto& distanceSensorDto, SensorPort port)
{
	Sonar* distanceSensor = new Sonar(mapSensorPort(port), connection);
	distanceSensorDto.port = port;
	distanceSensors.emplace(port, distanceSensor);
}

/// <summary>
/// Initializes a motor
/// </summary>
/// <param name="port">Port of the motor on the brick</param>
motor_dto communication::initializeMotor(MotorPort port)
{
	Motor* motor = new Motor(mapMotorPort(port), connection);
	motors.emplace(port, motor);
	return motor_dto{ static_cast<unsigned int>(port) };
}

/// <summary>
/// Maps between the sensor dtos and the library objects
/// </summary>
/// <param name="port">Port of the sensor on the brick</param>
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

/// <summary>
/// Maps between the motor dtos and the library objects
/// </summary>
/// <param name="port">Port of the motor on the brick</param>
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

/// <summary>
/// Reads and updates the touch sensor value
/// </summary>
/// <param name="touchSensorDto">Sensor dto</param>
void communication::updateSensorValue(touch_sensor_dto& touchSensorDto)
{
	Touch* touchSensor = touchSensors[touchSensorDto.port];
	touchSensorDto.is_pressed = touchSensor->read();
}

/// <summary>
/// Reads and updates the color sensor value
/// </summary>
/// <param name="colorSensorDto">Sensor dto</param>
void communication::updateSensorValue(color_sensor_dto& colorSensorDto)
{
	color2* colorSensor = colorSensors[colorSensorDto.port];
	colorSensorDto.intensity = colorSensor->getValue();
}

/// <summary>
/// Reads and updates the distance sensor value
/// </summary>
/// <param name="distanceSensorDto">Sensor dto</param>
void communication::updateSensorValue(distance_sensor_dto& distanceSensorDto)
{
	Sonar* distanceSensor = distanceSensors[distanceSensorDto.port];
	int value = distanceSensor->read();
	distanceSensorDto.distance = value;
}

/// <summary>
/// Stops all initialized motors connected to the brick
/// </summary>
void communication::stopAllMotors() {
	for_each(motors.begin(), motors.end(), [](pair<int, Motor*> pair) { pair.second->stop(); });
}

/// <summary>
/// Resets all color sensors connected to the brick
/// </summary>
void communication::resetSensors() {
	//for_each(colorSensors.begin(), colorSensors.end(), [](pair<int, Sensor*> pair) { pair.second->reset(); });
}

/// <summary>
/// Determines if a motor is currently running
/// </summary>
/// <param name="motorDto">Motor to verify</param>
bool communication::isMotorRunning(motor_dto motorDto)
{
	Motor* motor = motors[motorDto.port];
	return motor->is_running();
}

/// <summary>
/// Starts a motor (on)
/// </summary>
/// <param name="motorDto">Motor to start</param>
/// <param name="speed">Speed of the motor (0-100)</param>
/// <param name="degrees">Degrees of rotation (0-360)</param>
/// <param name="reply">Force NXT to reply (false by default)</param>
void communication::startMotor(motor_dto motorDto, char speed, unsigned int degrees /* = 0 */, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->on(speed, degrees, reply);
}

/// <summary>
/// Coasts a motor (off) (motor will not brake)
/// </summary>
/// <param name="motorDto">Motor to coast</param>
/// <param name="reply">Force NXT to reply (false by default)</param>
void communication::coastMotor(motor_dto motorDto, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->off(reply);
}

/// <summary>
/// Stops a motor (brake)
/// </summary>
/// <param name="motorDto">Motor to stop</param>
/// <param name="reply">Force NXT to reply (false by default)</param>
void communication::stopMotor(motor_dto motorDto, bool reply /* = false */)
{
	Motor* motor = motors[motorDto.port];
	motor->stop(reply);
}
