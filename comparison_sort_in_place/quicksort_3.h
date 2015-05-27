// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

namespace comparisonSortInPlace {

template <typename T, typename Predicate, typename Size, typename Method>
void quicksort3(T *start, T *end, Predicate less, Size minSize, Method nextMethod) {
  auto length = end - start;
  if (length <= minSize) {
    nextMethod(start, end);
    return;
  }
  T pivot = start[length / 2];
  T *a = start;  // [start, a) is <pivot region
  T *b = start;  // [b, ptr) is >pivot region
  for (T *ptr = start; ptr < end; ptr++) {
    if (less(pivot, *ptr))
      continue;
    T v = std::move(*ptr);
    *ptr = std::move(*b++);  // >pivot region shifted
    if (less(v, pivot))
      *a++ = std::move(v);  // <pivot region extended
  }
  std::fill(a, b, pivot);
  quicksort3(start, a, less, minSize, nextMethod);  // Sort <pivot region
  quicksort3(b, end, less, minSize, nextMethod);  // Sort >pivot region
}
}