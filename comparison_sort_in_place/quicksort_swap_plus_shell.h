// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include "stddef.h"
#include "shell_sort.h"

namespace comparisonSortInPlace {

template<typename T>
void quicksortSwapPlusShell(T *start, T *end) {
  auto length = end - start;
  if (length <= 250)
    return shellSort(start, end);

  T pivot = start[length / 2];
  T *lt = start;
  T *gt = end;

  while (lt < gt) {
    T a = *lt;
    if (a >= pivot) {
      T b;
      do {
        gt--;
        if (gt == lt)
          goto escape;
        b = *gt;
      } while (b >= pivot);

      *lt = b;
      *gt = a;
    }
    lt++;
  }

  escape:;

  if (start == lt) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (*ptr != pivot)
        continue;
      *ptr = *lt;
      *lt++ = pivot;
    }
  } else {
    quicksortSwapPlusShell(start, lt);
  }
  quicksortSwapPlusShell(lt, end);
}

}