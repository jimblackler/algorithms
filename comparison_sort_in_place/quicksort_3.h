// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include "stddef.h"

namespace comparisonSortInPlace {

template<typename T>
void quicksort3(T *start, T *end) {
  auto length = end - start;
  if (length <= 1)
    return;
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
  for (T *ptr = a; ptr < b; ptr++)
    *ptr = pivot;  // Fill =pivot region
  quicksort3(start, a);  // Sort <pivot region
  quicksort3(b, end);  // Sort >pivot region
}
}