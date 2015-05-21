// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_3.h"

#include <thread>
#include <stddef.h>

namespace comparisonSortInPlace {

template<typename T>
void quicksort3Threaded(T *start, T *end, T minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksort3(start, end);
  T pivot = start[length / 2];
  T *a = start;  // [start, a) is <pivot region
  T *b = start;  // [b, ptr) is >pivot region
  for (T *ptr = start; ptr < end; ptr++) {
    T v = *ptr;
    if (v > pivot)
      continue;
    *ptr = *b++;  // >pivot region shifted
    if (v != pivot)
      *a++ = v;  // <pivot region extended
  }

  std::thread t1([=]() {
      quicksort3Threaded(start, a, minSize);  // Sort <pivot region
  });

  for (T *ptr = a; ptr < b; ptr++)
    *ptr = pivot;  // Fill =pivot region

  quicksort3Threaded(b, end, minSize);  // Sort >pivot region
  t1.join();
}
}