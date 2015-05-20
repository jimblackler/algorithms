// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_3_threaded.h"

#include "quicksort_3.h"

#include <thread>

namespace comparisonSortInPlace {

void quicksort3Threaded(int *start, int *end, int minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksort3(start, end);
  int pivot = start[length / 2];
  int *a = start;  // [start, a) is <pivot region
  int *b = start;  // [b, ptr) is >pivot region
  for (int *ptr = start; ptr < end; ptr++) {
    int v = *ptr;
    if (v > pivot)
      continue;
    *ptr = *b++;  // >pivot region shifted
    if (v != pivot)
      *a++ = v;  // <pivot region extended
  }

  std::thread t1([=]() {
      quicksort3Threaded(start, a, minSize);  // Sort <pivot region
  });

  for (int *ptr = a; ptr < b; ptr++)
    *ptr = pivot;  // Fill =pivot region

  quicksort3Threaded(b, end, minSize);  // Sort >pivot region
  t1.join();
}
}