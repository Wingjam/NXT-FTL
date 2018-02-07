#ifndef COMMUNICATION
#define COMMUNICATION

#include <string>
#include <array>

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

	bool connect();
	bool disconnect();
	bool initializeSensor(Sensor_port port, Sensor_type type);
	int getSensorValue(Sensor_port port);
	std::array<int, 3> getSensorRGB(Sensor_port port);
	std::string printSensorValue(Sensor_port port);
	bool initializeMotor(Motor_port port);
	long int getMotorRotation(Motor_port port);
	bool isMotorRunning(Motor_port port);
	void startMotor(Motor_port port, char speed, unsigned int degrees = 0, bool reply = false);
	void coastMotor(Motor_port port, char speed, unsigned int degrees = 0, bool reply = false);
	void stopMotor(Motor_port port, bool reply = false);
};

#endif