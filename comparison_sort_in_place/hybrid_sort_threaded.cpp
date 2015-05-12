// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "hybrid_sort_threaded.h"

#include "hybrid_sort.h"

#include <thread>

namespace comparisonSortInPlace {

void hybridSortThreaded(int *start, int *end, int minSize) {
  if (end - start <= minSize)
    return hybridSort(start, end);
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
      hybridSortThreaded(start, a, minSize);  // Sort <pivot region
  });

  hybridSortThreaded(b, end, minSize);  // Sort >pivot region
  t1.join();
}
}