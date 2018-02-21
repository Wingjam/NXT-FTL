

#pragma once

#include "../NxtLibrary/sensor.h"
#include "../NxtLibrary/connection.h"

class color2 : public Sensor
{
public:
	color2(Sensor_port port, Connection* connection) :Sensor(port, connection, COMPASS, RAW_MODE) {};
	~color2(){}

	int getValue() {
		return read();
	}
};

