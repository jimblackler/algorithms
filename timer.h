#include <mach/mach_types.h>

class Timer {

public:
  static Timer* newTimer();
  virtual long long getTime() = 0;

};

