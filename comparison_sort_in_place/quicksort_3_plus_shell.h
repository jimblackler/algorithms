// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "shell_sort.h"

namespace comparisonSortInPlace {

template<typename T, typename F>
void quicksort3PlusShell(T *start, T *end, F less) {
  auto length = end - start;
  if (length <= 250)
    return shellSort(start, end, less);
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
  quicksort3PlusShell(start, a, less);  // Sort <pivot region
  quicksort3PlusShell(b, end, less);  // Sort >pivot region
}
}