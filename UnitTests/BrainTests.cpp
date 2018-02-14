#include "pch.h"
#include "../NXT-FTL/Brain.h"
#include "../NXT-FTL/Brain.cpp"

/*

ASSERT_* yields a fatal failure and returns from the current function
EXPECT_* yields a nonfatal failure, allowing the function to continue running

							Error messages
ASSERT_TRUE(i == 1) << "This text is displayed if the assertion fails" << i;

							Basic operators
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

					Fixtures
Declaration :

class MyTestFixture : public ::testing::Test {
protected:
virtual void SetUp() {
// Code here will be called immediately after the constructor (right
// before each test).
}

virtual void TearDown() {
// Code here will be called immediately after each test (right
// before the destructor).
}

// Objects declared here can be used by all tests in the test case for Foo.
int my_int_var {};
};

Usage :
// IMPORTANT : Use TEST_F(fixture, caseName) instead of TEST(test, caseName)
// Name of test must be name of fixture (ex: MyTestFixture)
TEST_F(MyTestFixture, AnyCustomTestCaseName) {
	// Members of the fixtures are available directly as if they were declared locally.
	EXPECT_EQ(my_int_var, 0);
}

*/

static unsigned int STOP_DISTANCE = 10;
static unsigned int COLOR_VALUE = 10;

class BrainTestFixture : public ::testing::Test {
protected:
	virtual void SetUp() {
		touch.isPressed = false;
		distance.Distance = 15;
		left.BlueValue = COLOR_VALUE;
		left.RedValue = COLOR_VALUE;
		left.GreenValue = COLOR_VALUE;
		right.BlueValue = COLOR_VALUE;
		right.RedValue = COLOR_VALUE;
		right.GreenValue = COLOR_VALUE;
	}

	Brain brain{ STOP_DISTANCE };
	TouchSensorDto touch{};
	DistanceSensorDto distance{};
	ColorSensorDto left{};
	ColorSensorDto right{};
};

TEST_F(BrainTestFixture, BrainStopOnDistance) {
	distance.Distance = 5;

	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_FALSE(std::get<1>(directionResult));
	EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST_F(BrainTestFixture, BrainStopOnTouch) {
	touch.isPressed = true;

	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_FALSE(std::get<1>(directionResult));
	EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST_F(BrainTestFixture, BrainGoLeft) {
	right.BlueValue = COLOR_VALUE * 2;
	right.RedValue = COLOR_VALUE * 2;
	right.GreenValue = COLOR_VALUE * 2;

	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_TRUE(std::get<1>(directionResult));
	EXPECT_GT(0, std::get<0>(directionResult));
}

TEST_F(BrainTestFixture, BrainGoRight) {
	left.BlueValue = COLOR_VALUE * 2;
	left.RedValue = COLOR_VALUE * 2;
	left.GreenValue = COLOR_VALUE * 2;

	auto directionResult = brain.ComputeDirection(touch, distance, left, right);

	EXPECT_TRUE(std::get<1>(directionResult));
	EXPECT_LT(0, std::get<0>(directionResult));
}