#include <sys/time.h>

long long int getMicroseconds() {
  struct timeval time;
  gettimeofday(&time, 0);
  long long before = 1000000 * time.tv_sec + time.tv_usec;
  return before;
}