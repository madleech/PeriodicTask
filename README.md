# PeriodicTask

[![CI](https://github.com/madleech/PeriodicTask/actions/workflows/ci.yml/badge.svg)](https://github.com/madleech/PeriodicTask/actions/workflows/ci.yml)

Run tasks periodically without blocking, using `millis()`.

`PeriodicTask` is a tiny, header-light scheduler for Arduino. Give it a period,
call `tick()` from `loop()`, and it returns `true` each time the period elapses —
no `delay()`, so the rest of your loop keeps running.

## Install

**Arduino Library Manager:** search for *PeriodicTask* and click Install.

**PlatformIO** (`platformio.ini`):

```ini
lib_deps = madleech/PeriodicTask
```

**Manual:** copy this folder into your Arduino `libraries/` directory.

## Usage

```cpp
#include <PeriodicTask.h>

PeriodicTask blinker(500);  // every 500 ms
bool on = false;

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	if (blinker.tick())
	{
		on = !on;
		digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
	}

	// other non-blocking work runs every loop()
}
```

See [`examples/Blink`](examples/Blink) for the full sketch. For a one-shot,
on-demand task (armed with `next_run_in()` and kept dormant with `stop()`), see
[`examples/DeferredTask`](examples/DeferredTask).

## API

| Method | Description |
| --- | --- |
| `PeriodicTask(uint32_t period = 1000)` | Construct a task with a period in milliseconds. |
| `bool tick()` | Returns `true` once the period has elapsed, then reschedules. Call every `loop()`. |
| `void reset()` | Restart the period from now (delay the next run by one full period). |
| `void run_next_time()` | Fire on the next `tick()`. |
| `void next_run_in(uint32_t delay)` | Schedule the next run `delay` ms from now. |
| `void next_run_at(uint32_t next_tick)` | Schedule the next run at an absolute `millis()` value. |
| `void stop()` | Suspend the task; `tick()` returns `false` until resumed. |
| `void resume()` | Resume a stopped task (next run one period out). |
| `uint32_t period` | The period, in milliseconds. Writable. |

## Notes

Timing is based on the unsigned `millis()` counter, which wraps roughly every
49 days. Comparisons use absolute time, so very long-running tasks may behave
unexpectedly across a wrap.

## License

MIT — see [LICENSE](LICENSE).
