#include "pch.h"
#include "../NXT-FTL/Brain.h"
#include "../NXT-FTL/Brain.cpp"

/*

ASSERT_* yields a fatal failure and returns from the current function
EXPECT_* yields a nonfatal failure, allowing the function to continue running

Basic operators :
ASSERT_TRUE(condition)
ASSERT_FALSE(condition)
ASSERT_EQ(val1, val2)
ASSERT_NE(val1, val2)
ASSERT_LT(val1, val2)
ASSERT_LE(val1, val2)
ASSERT_GT(val1, val2)
ASSERT_GE(val1, val2)

EXPECT_TRUE(condition)
EXPECT_FALSE(condition)
EXPECT_EQ(val1, val2)
EXPECT_NE(val1, val2)
EXPECT_LT(val1, val2)
EXPECT_LE(val1, val2)
EXPECT_GT(val1, val2)
EXPECT_GE(val1, val2)

String operators :
ASSERT_STREQ(str1, str2)
ASSERT_STRNE(str1, str2)
ASSERT_STRCASEEQ(str1, str2) : ignores case
ASSERT_STRCASENE(str1, str2) : ignores case

EXPECT_STREQ(str1, str2)
EXPECT_STRNE(str1, str2)
EXPECT_STRCASEEQ(str1, str2) : ignores case
EXPECT_STRCASENE(str1, str2) : ignores case

*/

static unsigned int STOP_DISTANCE = 10;

TEST(BrainTest, BrainInitialization) {
	Brain brain = Brain{ STOP_DISTANCE };
}

TEST(BrainTest, BrainStopOnDistance) {
	const unsigned int colorValue = 10;
	Brain brain{ STOP_DISTANCE };
	TouchSensorDto touch{};
	touch.isPressed = false;
	DistanceSensorDto distance{};
	distance.Distance = 5;
	ColorSensorDto left{};
	left.BlueValue = colorValue;
	left.RedValue = colorValue;
	left.GreenValue = colorValue;
	ColorSensorDto right{};
	right.BlueValue = colorValue;
	right.RedValue = colorValue;
	right.GreenValue = colorValue;
	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_FALSE(std::get<1>(directionResult));
	EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST(BrainTest, BrainStopOnTouch) {
	const unsigned int colorValue = 10;
	Brain brain{ STOP_DISTANCE };
	TouchSensorDto touch{};
	touch.isPressed = true;
	DistanceSensorDto distance{};
	distance.Distance = 15;
	ColorSensorDto left{};
	left.BlueValue = colorValue;
	left.RedValue = colorValue;
	left.GreenValue = colorValue;
	ColorSensorDto right{};
	right.BlueValue = colorValue;
	right.RedValue = colorValue;
	right.GreenValue = colorValue;
	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_FALSE(std::get<1>(directionResult));
	EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST(BrainTest, BrainGoLeft) {
	const unsigned int colorValue = 10;
	Brain brain{ STOP_DISTANCE };
	TouchSensorDto touch{};
	touch.isPressed = false;
	DistanceSensorDto distance{};
	distance.Distance = 15;
	ColorSensorDto left{};
	left.BlueValue = colorValue;
	left.RedValue = colorValue;
	left.GreenValue = colorValue;
	ColorSensorDto right{};
	right.BlueValue = colorValue * 2;
	right.RedValue = colorValue * 2;
	right.GreenValue = colorValue * 2;
	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_TRUE(std::get<1>(directionResult));
	EXPECT_GT(0, std::get<0>(directionResult));
}

TEST(BrainTest, BrainGoRight) {
	const unsigned int colorValue = 10;
	Brain brain{ STOP_DISTANCE };
	TouchSensorDto touch{};
	touch.isPressed = false;
	DistanceSensorDto distance{};
	distance.Distance = 15;
	ColorSensorDto left{};
	left.BlueValue = colorValue * 2;
	left.RedValue = colorValue * 2;
	left.GreenValue = colorValue * 2;
	ColorSensorDto right{};
	right.BlueValue = colorValue;
	right.RedValue = colorValue;
	right.GreenValue = colorValue;
	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_TRUE(std::get<1>(directionResult));
	EXPECT_LT(0, std::get<0>(directionResult));
}