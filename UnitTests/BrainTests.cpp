#include "pch.h"
#include "../NXT-FTL/brain.h"
#include "../NXT-FTL/color_sensor_dto.h"
#include "../NXT-FTL/distance_sensor_dto.h"
#include "../NXT-FTL/touch_sensor_dto.h"

using namespace nxtftl;

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
	brain brain{ STOP_DISTANCE };
}

TEST(BrainTest, BrainStopOnDistance) {
	const unsigned int colorValue = 10;
	brain brain{ STOP_DISTANCE };
	touch_sensor_dto touch{};
	touch.isPressed = false;
	distance_sensor_dto distance{};
	distance.Distance = 5;
	color_sensor_dto left{};
	left.blue_value = colorValue;
	left.red_value = colorValue;
	left.green_value = colorValue;
	color_sensor_dto right{};
	right.blue_value = colorValue;
	right.red_value = colorValue;
	right.green_value = colorValue;
	auto directionResult = brain.compute_direction(touch, distance, left, right);

	EXPECT_FALSE(std::get<1>(directionResult));
	EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST(BrainTest, BrainStopOnTouch) {
	const unsigned int colorValue = 10;
	brain brain{ STOP_DISTANCE };
	touch_sensor_dto touch{};
	touch.isPressed = true;
	distance_sensor_dto distance{};
	distance.Distance = 15;
	color_sensor_dto left{};
	left.blue_value = colorValue;
	left.red_value = colorValue;
	left.green_value = colorValue;
	color_sensor_dto right{};
	right.blue_value = colorValue;
	right.red_value = colorValue;
	right.green_value = colorValue;
	auto directionResult = brain.compute_direction(touch, distance, left, right);

	EXPECT_FALSE(std::get<1>(directionResult));
	EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST(BrainTest, BrainGoLeft) {
	const unsigned int colorValue = 10;
	brain brain{ STOP_DISTANCE };
	touch_sensor_dto touch{};
	touch.isPressed = false;
	distance_sensor_dto distance{};
	distance.Distance = 15;
	color_sensor_dto left{};
	left.blue_value = colorValue;
	left.red_value = colorValue;
	left.green_value = colorValue;
	color_sensor_dto right{};
	right.blue_value = colorValue * 2;
	right.red_value = colorValue * 2;
	right.green_value = colorValue * 2;
	auto directionResult = brain.compute_direction(touch, distance, left, right);

	EXPECT_TRUE(std::get<1>(directionResult));
	EXPECT_GT(0, std::get<0>(directionResult));
}

TEST(BrainTest, BrainGoRight) {
	const unsigned int colorValue = 10;
	brain brain{ STOP_DISTANCE };
	touch_sensor_dto touch{};
	touch.isPressed = false;
	distance_sensor_dto distance{};
	distance.Distance = 15;
	color_sensor_dto left{};
	left.blue_value = colorValue * 2;
	left.red_value = colorValue * 2;
	left.green_value = colorValue * 2;
	color_sensor_dto right{};
	right.blue_value = colorValue;
	right.red_value = colorValue;
	right.green_value = colorValue;
	auto directionResult = brain.compute_direction(touch, distance, left, right);

	EXPECT_TRUE(std::get<1>(directionResult));
	EXPECT_LT(0, std::get<0>(directionResult));
}