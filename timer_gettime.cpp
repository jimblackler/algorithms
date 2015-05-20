#include "timer.h"

#include <time.h>

class TimerGettime : public Timer {
private:

public:
  TimerGettime() {

  };

  ~TimerGettime() {

  }

  long long getTime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000000000 + time.tv_nsec;
  }
};

Timer *Timer::newTimer() {
  return new TimerGettime();
}