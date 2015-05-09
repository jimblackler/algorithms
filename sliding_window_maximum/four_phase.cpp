// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "four_phase.h"

#include <deque>

namespace slidingWindowMaximum {
  
void fourPhase(const int *in, size_t length, size_t windowSize, int *out) {
  std::deque<int> queue((size_t) windowSize);

  queue.push_front(0);
  out[0] = in[0];
  for (int i = 1; i < windowSize; i++) {
    int value = in[i];
    while (in[queue.front()] <= value) {
      queue.pop_front();
      if (queue.empty())
        break;
    }
    queue.push_front(i);
    out[i] = in[queue.back()];
  }

  int lengthMinusK = length - windowSize;
  for (int i = windowSize; i < lengthMinusK; i++) {
    if (queue.back() == i - windowSize) {
      queue.pop_back();
    }
    int value = in[i];
    while (in[queue.front()] <= value) {
      queue.pop_front();
      if (queue.empty())
        break;
    }
    queue.push_front(i);
    out[i] = in[queue.back()];
  }

  int i = lengthMinusK;
  while (true) {
    if (queue.back() == i - windowSize) {
      queue.pop_back();
    }
    int value = in[i];
    while (in[queue.front()] <= value) {
      queue.pop_front();
      if (queue.empty())
        break;
    }
    queue.push_front(i);
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