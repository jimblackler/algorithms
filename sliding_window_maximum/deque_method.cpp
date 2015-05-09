// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "deque_method.h"

#include <deque>

namespace slidingWindowMaximum {

void dequeMethod(const int *in, size_t length, size_t windowSize, int *out) {
  std::deque<int> queue;

  queue.push_front(0);
  out[0] = in[0];
  for (int i = 1; i < length; i++) {
    if (queue.back() + windowSize == i) {
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
}

}