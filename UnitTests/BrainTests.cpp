#include "pch.h"
#include "../NXT-FTL/Brain.h"
#include "../NXT-FTL/Brain.cpp"

static unsigned int STOP_DISTANCE = 10;

TEST(BrainTest, BrainInitialization) {
	Brain brain = Brain{ STOP_DISTANCE };
}