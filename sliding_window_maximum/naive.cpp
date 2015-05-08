// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <algorithm>
#include "naive.h"

namespace slidingWindowMaximum {

void naive(const int *in, size_t length, size_t windowSize, int *out) {

  for (int i = 0; i < length; i++) {
    int v = 0;
    for (int j = i; j >= 0 && j > i - windowSize; j--) {
      v = std::max(v, in[j]);
    }
    out[i] = v;
  }
}

}