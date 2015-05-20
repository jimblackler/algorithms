#include <mach/mach_types.h>

class Timer {
  clock_serv_t clock;
public:
  long long getTime();
  Timer();
  ~Timer();

};

