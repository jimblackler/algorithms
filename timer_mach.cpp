#include <mach/mach_host.h>
#include <mach/clock.h>
#include <mach/mach_port.h>
#include "timer.h"

class TimerMach : public Timer {
private:
  clock_serv_t clock;
public:
  TimerMach() {
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock);
  };

  ~TimerMach() {
    mach_port_deallocate(mach_host_self(), clock);
  }

  long long getTime() {
    mach_timespec_t mts;
    clock_get_time(clock, &mts);
    return 1000000000LL * mts.tv_sec + mts.tv_nsec;
  }
};

Timer *Timer::newTimer() {
  return new TimerMach();
}