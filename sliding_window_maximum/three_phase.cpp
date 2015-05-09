// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "three_phase.h"

#include <deque>

namespace slidingWindowMaximum {
  
void threePhase(const int *in, size_t length, size_t windowSize, int *out) {
  std::deque<int> queue;

  queue.push_front(0);
  out[0] = in[0];
  size_t lengthMinusK = length - windowSize;
  for (int i = 1; i < lengthMinusK; i++) {
    int value = in[i];
    while (in[queue.front()] <= value) {
      queue.pop_front();
      if (queue.empty())
        break;
    }
    queue.push_front(i);
    if (queue.back() == i - windowSize) {
      queue.pop_back();
    }
    out[i] = in[queue.back()];
  }

  int i = (int) lengthMinusK;
  while (i < length) {
    int value = in[i];
    while (in[queue.front()] <= value) {
      queue.pop_front();
      if (queue.empty())
        break;
    }
    queue.push_front(i);
    if (queue.back() == i - windowSize) {
      queue.pop_back();
    }
    int back = queue.back();
    out[i] = in[back];
    i++;
    if (back >= lengthMinusK)
      break;
  }

  int m = in[queue.back()];

  while (i < length) {
    m = std::max(m, in[i]);
    out[i] = m;
    i++;
  }

}
}