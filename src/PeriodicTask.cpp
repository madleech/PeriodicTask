/*
    PeriodicTask - ...
    Copyright (C) 2016 Michael Adams (www.michael.net.nz)
    All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <stdint.h>

#include "PeriodicTask.h"
#include "Arduino.h"

PeriodicTask::PeriodicTask(uint32_t period)
    : period(period), _next_tick(period), _stopped(false)
{
}

// detect whether we should run this time or not
bool PeriodicTask::tick()
{
	if (_stopped)
		return false;

	// Compare elapsed time using modular (wraparound-safe) arithmetic rather
	// than absolute values. The unsigned subtraction wraps cleanly when millis()
	// rolls over (roughly every 49 days), and the signed cast tells us whether
	// we have reached _next_tick yet. This is correct as long as the scheduled
	// interval stays below ~24.8 days (INT32_MAX milliseconds).
	if ((int32_t)((uint32_t)millis() - _next_tick) >= 0)
	{
		reset();
		return true;
	}
	else
	{
		return false;
	}
}

// tell this task to run asap
void PeriodicTask::run_next_time()
{
	_next_tick = millis();
	_stopped = false;
}

// when to run the task again (absolute time)
void PeriodicTask::next_run_at(uint32_t next_tick)
{
	_next_tick = next_tick;
	_stopped = false;
}

// when to run the task again (relative time)
void PeriodicTask::next_run_in(uint32_t delay)
{
	_next_tick = millis() + delay;
	_stopped = false;
}

// reset elapsed time to zero, i.e. delay running this task by its period
void PeriodicTask::reset()
{
	_next_tick = millis() + period;
}

// stop running this task
void PeriodicTask::stop()
{
	_stopped = true;
}

// start running again
void PeriodicTask::resume()
{
	if (_stopped)
	{
		_stopped = false;
		reset();
	}
}
