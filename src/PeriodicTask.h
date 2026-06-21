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

#ifndef _PeriodicTask_h
#define _PeriodicTask_h

#include "Arduino.h"

class PeriodicTask
{
  public:
	PeriodicTask(uint32_t period = 1000);

	bool tick();
	void run_next_time();
	void next_run_at(uint32_t next_tick);
	void next_run_in(uint32_t delay);
	void reset();
	void stop();
	void resume();

	uint32_t period;

  private:
	uint32_t _next_tick;
	bool _stopped;
};

#endif
