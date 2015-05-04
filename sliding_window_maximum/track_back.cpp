#include "track_back.h"

namespace slidingWindowMaximum {

void trackBack(const int *in, size_t length, size_t windowSize, int *out) {

  int from = 0;
  for (size_t i = 0; i < length; i++) {
    if (in[i] >= in[from]) {
      from = i;
    }
    out[i] = in[from];
    if (i < from + windowSize - 1)
      continue;

    from++;
    for (size_t f = from + 1; f <= i; f++) {
      if (in[f] >= in[from]) {
        from = f;
      }
    }
  }
}

}