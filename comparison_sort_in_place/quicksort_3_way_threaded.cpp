// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_3_way_threaded.h"

#include "quicksort_3_way.h"

#include <deque>
#include <ostream>
#include <thread>
#include <iostream>

namespace comparisonSortInPlace {

void quicksort3WayThreaded(int *start, int *end, int minSize) {
  if (end - start <= minSize)
    return quicksort3Way(start, end);
  int pivot = *(start + (end - start) / 2);
  int *a = start;  // [start, a) is <pivot region
  int *b = start;  // [b, ptr) is >pivot region
  for (int *ptr = start; ptr < end; ptr++) {
    int v = *ptr;
    if (v > pivot)
      continue;
    *ptr = *b++;  // >pivot region shifted
    if (v < pivot)
      *a++ = v;  // <pivot region extended
  }
  for (int *ptr = a; ptr < b; ptr++)
    *ptr = pivot;  // Fill =pivot region

  std::thread t1([=]() {
      quicksort3WayThreaded(start, a, minSize);  // Sort <pivot region
  });

  quicksort3WayThreaded(b, end, minSize);  // Sort >pivot region
  t1.join();
}
}