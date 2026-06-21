/*
    Native unit tests for PeriodicTask.

    These run off-device via PlatformIO's `native` platform. millis() is mocked
    (see test/mock/Arduino.h) so we can drive time forward deterministically.
*/

#include <unity.h>

#include "Arduino.h"
#include "PeriodicTask.h"

// The mock clock declared in test/mock/Arduino.h.
unsigned long _mock_millis = 0;

unsigned long millis()
{
	return _mock_millis;
}

void setUp(void)
{
	_mock_millis = 0;
}

void tearDown(void)
{
}

// Does not fire before the period has elapsed.
void test_does_not_fire_early(void)
{
	PeriodicTask task(1000);
	_mock_millis = 999;
	TEST_ASSERT_FALSE(task.tick());
}

// Fires once the period has elapsed, then reschedules for the next period.
void test_fires_after_period_and_reschedules(void)
{
	PeriodicTask task(1000);

	_mock_millis = 1000;
	TEST_ASSERT_TRUE(task.tick());

	// Immediately after firing it should not fire again.
	TEST_ASSERT_FALSE(task.tick());

	// ...until another full period passes.
	_mock_millis = 2000;
	TEST_ASSERT_TRUE(task.tick());
}

// run_next_time() makes the task fire on the very next tick.
void test_run_next_time_fires_immediately(void)
{
	PeriodicTask task(1000);
	task.run_next_time();
	_mock_millis = 0;
	TEST_ASSERT_TRUE(task.tick());
}

// next_run_in() schedules relative to now.
void test_next_run_in_relative(void)
{
	PeriodicTask task(1000);
	_mock_millis = 5000;
	task.next_run_in(100);

	_mock_millis = 5099;
	TEST_ASSERT_FALSE(task.tick());

	_mock_millis = 5100;
	TEST_ASSERT_TRUE(task.tick());
}

// next_run_at() schedules at an absolute time.
void test_next_run_at_absolute(void)
{
	PeriodicTask task(1000);
	task.next_run_at(250);

	_mock_millis = 249;
	TEST_ASSERT_FALSE(task.tick());

	_mock_millis = 250;
	TEST_ASSERT_TRUE(task.tick());
}

// stop() suppresses the task indefinitely; resume() brings it back.
void test_stop_and_resume(void)
{
	PeriodicTask task(1000);
	task.stop();

	_mock_millis = 1000000;
	TEST_ASSERT_FALSE(task.tick());

	task.resume();
	// resume() reschedules one period out from now.
	_mock_millis = 1000999;
	TEST_ASSERT_FALSE(task.tick());

	_mock_millis = 1001000;
	TEST_ASSERT_TRUE(task.tick());
}

// Timing stays correct across the 32-bit millis() rollover (~49 days).
// The deadline is scheduled just before the wrap so it falls on the far side;
// the old absolute comparison (millis() >= _next_tick) would have fired
// immediately here instead of waiting.
void test_survives_millis_wraparound(void)
{
	PeriodicTask task(1000);

	_mock_millis = 0xFFFFFF00; // 2^32 - 256, just before the wrap
	task.reset();              // deadline = (2^32 - 256) + 1000 -> 744 after wrap

	// Still before the deadline, even though millis() is huge and _next_tick is
	// tiny. Absolute comparison would wrongly fire here.
	_mock_millis = 0xFFFFFFF0;
	TEST_ASSERT_FALSE(task.tick());

	// millis() has wrapped past zero and reached the deadline (744).
	_mock_millis = 0x300; // 768
	TEST_ASSERT_TRUE(task.tick());
}

int main(int, char **)
{
	UNITY_BEGIN();
	RUN_TEST(test_does_not_fire_early);
	RUN_TEST(test_fires_after_period_and_reschedules);
	RUN_TEST(test_run_next_time_fires_immediately);
	RUN_TEST(test_next_run_in_relative);
	RUN_TEST(test_next_run_at_absolute);
	RUN_TEST(test_stop_and_resume);
	RUN_TEST(test_survives_millis_wraparound);
	return UNITY_END();
}
