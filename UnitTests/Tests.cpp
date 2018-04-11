#include "pch.h"
#include <thread>
#include "../NXT-FTL/position.h"
#include "../NXT-FTL/position.cpp"
#include "../NXT-FTL/brain.h"
#include "../NXT-FTL/brain.cpp"
#include "../NXT-FTL/movement_history.h"
#include "../NXT-FTL/movement_history.cpp"
#include "../NXT-FTL/hermite.h"
#include "../NXT-FTL/hermite.cpp"
#include "../NXT-FTL/buffer_manager.h"
#include "../NXT-FTL/buffer_manager.cpp"
#include "../NXT-FTL/utils.h"
#include "../NXT-FTL/utils.cpp"
#include "../NXT-FTL/wrap_around_iterator.h"
#include "../NXT-FTL/wrap_around_iterator.cpp"

using namespace nxtftl;

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
        touch.is_pressed = false;
        distance.distance = 15;
        left.intensity = COLOR_VALUE;
        right.intensity = COLOR_VALUE;
    }

    brain brain{ STOP_DISTANCE };
    touch_sensor_dto touch{};
    distance_sensor_dto distance{};
    color_sensor_dto left{};
    color_sensor_dto right{};
};

TEST_F(BrainTestFixture, BrainStopOnDistance) {
    distance.distance = 5;
    auto need_to_stop = brain.check_for_critical_stop(distance);
    EXPECT_TRUE(need_to_stop);
}

TEST_F(BrainTestFixture, BrainStopOnTouch) {
    touch.is_pressed = true;

    auto directionResult = brain.compute_direction(touch, left, right);

    EXPECT_TRUE(std::get<1>(directionResult));
    EXPECT_EQ(0, std::get<0>(directionResult));
}

TEST_F(BrainTestFixture, BrainGoLeft) {
    right.intensity *= 2;

    auto directionResult = brain.compute_direction(touch, left, right);

    EXPECT_FALSE(std::get<1>(directionResult));
    EXPECT_GT(0, std::get<0>(directionResult));
}

TEST_F(BrainTestFixture, BrainGoRight) {
    left.intensity *= 2;

    auto directionResult = brain.compute_direction(touch, left, right);

    EXPECT_FALSE(std::get<1>(directionResult));
    EXPECT_LT(0, std::get<0>(directionResult));
}

TEST(MovementHistoryTest, Rotation) {
    vector<position> output{5};
    buffer_manager<position> buffers{10, 100};
    wrap_around_iterator iter{ output.begin(), output.end() };
    export_to_multiple_buffers buffer_push_fct{ &buffers, &iter };
    movement_history mov_history{ buffer_push_fct, 0, 0 };
    mov_history.log_rotation(10, 20);
    position position_1 = mov_history.get_current_position();
    EXPECT_NE(position_1.direction_in_rad, 0.f);

    mov_history.log_rotation(30, 30);
    position position = mov_history.get_current_position();
    EXPECT_FLOAT_EQ(position.direction_in_rad, 0.f);
}

TEST(MovementHistoryTest, NewPosition) {
    vector<position> output{ 5 };
    buffer_manager<position> buffers{ 10, 100 };
    wrap_around_iterator iter{ output.begin(), output.end() };
    export_to_multiple_buffers buffer_push_fct{ &buffers, &iter };
    movement_history mov_history{ buffer_push_fct, 0, 0 };
    mov_history.log_rotation(10, 10);
    mov_history.log_rotation(20, 20);
    mov_history.log_rotation(30, 30);
    mov_history.log_rotation(40, 40);
    mov_history.log_rotation(50, 50);
    mov_history.log_rotation(60, 60);
    mov_history.log_rotation(70, 70);
    mov_history.log_rotation(80, 80);

    position position = mov_history.get_current_position();
    EXPECT_FLOAT_EQ(position.direction_in_rad, 0.f);
    EXPECT_FLOAT_EQ(position.y, 0.f);
    EXPECT_GT(position.x, 0.f);
}

TEST(BufferManager, WriteOnFullTest) {
    buffer_manager<position> buf_manager{ 1, 0 };
    EXPECT_FALSE(buf_manager.push_back(position{ 1,1,1 }));

    buffer_manager<position> buf_manager2{ 1, 1 };
    EXPECT_TRUE(buf_manager2.push_back(position{ 1,1,1 }));
    EXPECT_FALSE(buf_manager2.push_back(position{ 1,1,1 }));
}

TEST(BufferManager, SlowParallelAccess) {
    buffer_manager<position> buf_manager{ 2, 3 };
    atomic<bool> signal{ true };
    int expected_number_of_reads = 10;
    int number_of_reads = 0;
    // start another thread to read data
    std::thread t1([&buf_manager, &signal, &number_of_reads](){
        while (signal.load()) {
            while (buf_manager.get_current_read_buffer() == nullptr) { // Wait to get the read thread
                if (!signal.load()) {
                    return;
                }

                std::cerr << "sleep" << std::endl;
                std::this_thread::sleep_for(200ms);
            }
            std::cerr << "got the reading thread" << std::endl;

            std::vector<position>* current_read_buffer = buf_manager.get_current_read_buffer();

            EXPECT_NE(current_read_buffer, nullptr);

            std::for_each(std::begin(*current_read_buffer), std::end(*current_read_buffer), [&number_of_reads](position pos) {
                number_of_reads++;
                std::cerr << pos << " -- ";
            });

            std::cerr << std::endl;
            buf_manager.signal_current_buffer_completly_read();
        }
    });

    // insert data
    for (int i = 0; i < expected_number_of_reads; ++i) {
        EXPECT_TRUE(buf_manager.push_back(position{ 1,1,1 }));
        std::cerr << "insert" << std::endl;
        std::this_thread::sleep_for(100ms);
    }
    buf_manager.complete_adding();

    // Give some time to the reading thread to finish up
    std::this_thread::sleep_for(300ms);

    // Stop other thread
    signal.store(false);
    t1.join();

    EXPECT_EQ(number_of_reads, expected_number_of_reads);
}

TEST(BufferManager, FastParallelAccess) {
    buffer_manager<position> buf_manager{ 2, 3 };
    atomic<bool> signal{ true };
    int expected_number_of_reads = 0;
    int number_of_reads = 0;
    // start another thread to read data
    std::thread t1([&buf_manager, &signal, &number_of_reads]() {
        while (signal.load()) {
            while (buf_manager.get_current_read_buffer() == nullptr) { // Wait to get the read thread
                if (!signal.load()) {
                    return;
                }
            }
            std::cerr << "got the reading thread" << std::endl;

            std::vector<position>* current_read_buffer = buf_manager.get_current_read_buffer();

            std::for_each(std::begin(*current_read_buffer), std::end(*current_read_buffer), [&number_of_reads](position pos) {
                number_of_reads++;
                std::cerr << pos << " -- ";
            });

            std::cerr << std::endl;
            buf_manager.signal_current_buffer_completly_read();
        }
    });

    // insert data
    for (int i = 0; i < 10; ++i) {
        bool insert_result = buf_manager.push_back(position{ 1,1,1 });
        if (insert_result)
        { 
            expected_number_of_reads++;
            std::cerr << "inserted" << std::endl;
        }
        else 
        {
            std::cerr << "insert failed" << std::endl;
        }
    }
    buf_manager.complete_adding();

    // Give some time to the reading thread to finish up
    std::this_thread::sleep_for(300ms);

    // Stop other thread
    signal.store(false);
    t1.join();

    EXPECT_EQ(number_of_reads, expected_number_of_reads);
}

TEST(HermiteTest, HermiteSimpleTest) {
    position P1 = { 0.f, 1.f };
    position P2 = { 6.f, 1.f };
    std::vector<position> src = { P1, P2, position{} };
    std::vector<position> dest;

    auto inserter = [&dest](position pos) {dest.push_back(pos); };

    wrap_around_iterator read{ src.begin(), src.end() };
    auto resume_info = std::make_pair(&read, 0);
    wrap_around_iterator write{ src.begin(), src.end() };
    ++write;
    ++write;

    class myPred {
    public:
        bool operator()() const {
            return true;
        }
    };

    hermite hermite{};
    auto res = hermite.get_points_between_subdivided(resume_info, write, inserter, myPred{}, 4);

    EXPECT_EQ(P1, dest[0]);
    EXPECT_EQ(position(3.f, 1.f), dest[2]);
    EXPECT_EQ(dest.size(), 4);
}


TEST(UtilsTest, increment_value) {
    EXPECT_EQ(1, utils::increment_value(0, 10));
    EXPECT_EQ(0, utils::increment_value(10, 10));
    EXPECT_EQ(10, utils::increment_value(9, 10));
}

TEST(UtilsTest, distance_behind) {
    EXPECT_EQ(10, utils::distance_behind(0, 10, 20));
    EXPECT_EQ(5, utils::distance_behind(5, 10, 20));
    EXPECT_EQ(16, utils::distance_behind(10, 5, 20));
}
