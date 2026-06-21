# PeriodicTask

[![CI](https://github.com/madleech/PeriodicTask/actions/workflows/ci.yml/badge.svg)](https://github.com/madleech/PeriodicTask/actions/workflows/ci.yml)

Run tasks periodically without blocking, using `millis()`.

`PeriodicTask` is a tiny task scheduler for Arduino. It allows writing clean code to run a block of code periodically, without having to keep track of state, worry around rollover/wraparound, correct types, etc.

As an example, imagine you want to print some debug state every 30s. You can very cleanly create a `PeriodicTask debug_task(30000)`, then in your `loop()` function you can just use `if (debug_task.tick()) { ... }`. Nice and simple.

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

PeriodicTask hello_task(1000);  // every 1000ms / 1s

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	if (hello_task.tick())
	{
		Serial.println("Hello world");
	}

	// other non-blocking work runs every loop()
}
```

See [`examples/Blink`](examples/Blink) for a full example. For a one-shot,
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

## License

MIT — see [LICENSE](LICENSE).
