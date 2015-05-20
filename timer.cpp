#include <mach/mach_host.h>
#include <mach/clock.h>
#include <mach/mach_port.h>
#include "timer.h"

Timer::Timer() {
  host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock);
};

Timer::~Timer() {
  mach_port_deallocate(mach_host_self(), clock);
}

long long Timer::getTime() {
  mach_timespec_t mts;
  clock_get_time(clock, &mts);
  return mts.tv_sec * 1000000000 + mts.tv_nsec;
}