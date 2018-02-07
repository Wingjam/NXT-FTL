#pragma once
#include "FTLTouchSensor.h"
#include "FTLDistanceSensor.h"
#include "FTLColorSensor.h"

class Brain
{
public:
	Brain();
	~Brain();
	int ComputeDirection(FTLTouchSensor, FTLDistanceSensor, FTLColorSensor, FTLColorSensor);
};

