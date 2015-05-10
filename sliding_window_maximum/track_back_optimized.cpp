// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "track_back.h"

#include <climits>

namespace slidingWindowMaximum {

void trackBackOptimized(const int *in, size_t length, size_t windowSize, int *out) {
  int from = 0;
  int max = in[from];
  for (int i = 0; i < length; i++) {
    int v = in[i];
    if (v >= max) {
      from = i;
      max = v;
    }
    out[i] = max;
    if (i < from + windowSize - 1)
      continue;
    
    from++;

    max = INT_MIN;
    for (int f = from; f <= i; f++) {
      int v2 = in[f];
      if (v2 >= max) {
        from = f;
        max = v2;
      }
    }
  }
}

}