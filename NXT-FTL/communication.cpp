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

	return connected;
}

bool communication::connect_with_bluetooth(unsigned int comport) 
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
		stop_all_motors();
		connection->disconnect();
		cout << "NXT disconnected successfully." << endl;
	}
	catch (Nxt_exception& e) {
		print_error(e);
		connection->disconnect();
	}
	return success;
}

void communication::print_error(Nxt_exception& e)
{
	cout << e.what() << endl;
	cout << "error code: " << e.error_code() << endl;
	cout << "error type: " << e.error_type() << endl;
	cout << e.who() << endl;
}

void communication::initialize_sensor(touch_sensor_dto & touch_sensor_dto, SensorPort port)
{
	Touch* touch_sensor = new Touch(map_sensor_port(port), connection);
	touch_sensor_dto.port = port;
	touch_sensors.emplace(port, touch_sensor);
}

void communication::initialize_sensor(color_sensor_dto& color_ensor_dto, SensorPort port)
{
	color2* color_sensor = new color2(map_sensor_port(port), connection);
	color_ensor_dto.port = port;
	color_sensors.emplace(port, color_sensor);
}

void communication::initialize_sensor(distance_sensor_dto& distance_sensor_dto, SensorPort port)
{
	Sonar* distance_sensor = new Sonar(map_sensor_port(port), connection);
	distance_sensor_dto.port = port;
	distance_sensors.emplace(port, distance_sensor);
}

motor_dto communication::initialize_motor(MotorPort port)
{
	Motor* motor = new Motor(map_motor_port(port), connection);
	motors.emplace(port, motor);
	return motor_dto{ static_cast<unsigned int>(port) };
}

Sensor_port communication::map_sensor_port(SensorPort port) 
{
	Sensor_port mapped_port;
	switch (port)
	{
		case SensorPort::IN_1:
			mapped_port = Sensor_port::IN_1;
			break;
		case SensorPort::IN_2:
			mapped_port = Sensor_port::IN_2;
			break;
		case SensorPort::IN_3:
			mapped_port = Sensor_port::IN_3;
			break;
		case SensorPort::IN_4:
			mapped_port = Sensor_port::IN_4;
			break;
	}
	return mapped_port;
}

Motor_port communication::map_motor_port(MotorPort port)
{
	Motor_port mapped_port;
	switch (port)
	{
		case MotorPort::OUT_A:
			mapped_port = Motor_port::OUT_A;
			break;
		case MotorPort::OUT_B:
			mapped_port = Motor_port::OUT_B;
			break;
		case MotorPort::OUT_C:
			mapped_port = Motor_port::OUT_C;
			break;
	}
	return mapped_port;
}

void communication::update_sensor_value(touch_sensor_dto& touch_sensor_dto)
{
	Touch* touch_sensor = touch_sensors[touch_sensor_dto.port];
	touch_sensor_dto.is_pressed = touch_sensor->read();
}

void communication::update_sensor_value(color_sensor_dto& color_sensor_dto)
{
	color2* color_sensor = color_sensors[color_sensor_dto.port];
	color_sensor_dto.intensity = color_sensor->getValue();
}

void communication::update_sensor_value(distance_sensor_dto& distance_sensor_dto)
{
	Sonar* distance_sensor = distance_sensors[distance_sensor_dto.port];
	int value = distance_sensor->read();
	distance_sensor_dto.distance = value;
}

void communication::stop_all_motors() {
	for_each(motors.begin(), motors.end(), [](pair<int, Motor*> pair) { pair.second->stop(); });
}


bool communication::is_motor_running(motor_dto motor_dto)
{
	Motor* motor = motors[motor_dto.port];
	return motor->is_running();
}

void communication::start_motor(motor_dto motor_dto, char speed, unsigned int degrees /* = 0 */, bool reply /* = false */)
{
	Motor* motor = motors[motor_dto.port];
	motor->on(speed, degrees, reply);
}

void communication::coast_motor(motor_dto motor_dto, bool reply /* = false */)
{
	Motor* motor = motors[motor_dto.port];
	motor->off(reply);
}

void communication::stop_motor(motor_dto motor_dto, bool reply /* = false */)
{
	Motor* motor = motors[motor_dto.port];
	motor->stop(reply);
}

void communication::update_tacho_count(motor_dto& motor_dto)
{
	Motor* motor = motors[motor_dto.port];
	motor->get_output_state();
	motor_dto.tacho_count = motor->tacho_count;
}
