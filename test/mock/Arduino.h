/*
    Minimal Arduino.h mock for native unit tests.

    Provides just enough of the Arduino runtime for PeriodicTask to build and
    run off-device. The test clock (_mock_millis) is defined in the test file
    and can be advanced to simulate the passage of time.
*/

#ifndef _PeriodicTask_test_Arduino_h
#define _PeriodicTask_test_Arduino_h

#include <stdint.h>
#include <stddef.h>

// Controllable test clock, defined by the test translation unit.
extern unsigned long _mock_millis;

unsigned long millis();

#endif
