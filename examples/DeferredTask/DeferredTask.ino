/*
    DeferredTask - run a task once, on demand, after a delay.

    Unlike Blink (which fires repeatedly), this task does nothing until
    something asks it to run. We stop() it in setup() so it never fires on its
    own, then arm it with next_run_in() to fire exactly once at a future time -
    like a lightweight, non-blocking "run this in 5 seconds" timer.

    Here the trigger is any incoming serial byte; in a real sketch it might be a
    button press, a sensor threshold, or a network message.
*/

#include <PeriodicTask.h>

// period is irrelevant while stopped; we drive timing with next_run_in().
PeriodicTask task;

void setup()
{
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);

	task.stop(); // never run automatically - wait to be triggered

	Serial.println("Send any character to run the task in 5 seconds.");
}

void loop()
{
	// Trigger: any incoming serial byte arms the task.
	if (Serial.available())
	{
		while (Serial.available())
			Serial.read(); // drain the input buffer

		task.next_run_in(5000); // fire once, 5 seconds from now
		Serial.println("Armed - task will run in 5 seconds.");
	}

	// tick() returns true exactly once, when the 5 seconds have elapsed.
	if (task.tick())
	{
		Serial.println("Task running!");
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

		// tick() auto-reschedules one period out, so stop() again to keep this
		// a one-shot. (Drop this line to repeat every `task.period` ms instead.)
		task.stop();
	}

	// loop() keeps running the whole time - nothing here blocks.
}
