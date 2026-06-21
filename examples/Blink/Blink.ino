/*
    Blink - blink the built-in LED every 500ms without blocking.

    Demonstrates the core PeriodicTask pattern: construct a task with a
    period, then call tick() every loop(). tick() returns true once the
    period has elapsed, and reschedules itself automatically.
*/

#include <PeriodicTask.h>

PeriodicTask blinker(500);
bool led_on = false;

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	if (blinker.tick())
	{
		led_on = !led_on;
		digitalWrite(LED_BUILTIN, led_on ? HIGH : LOW);
	}

	// loop() keeps running, free to do other non-blocking work here.
}
