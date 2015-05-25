// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include "insertion_sort.h"

#include <type_traits>

namespace comparisonSortInPlace {

template<typename T, typename F>
void quicksortSwapPlusShell(T *start, T *end, F less) {
  auto length = end - start;
  if (length <= 500)
    return shellSort(start, end, less);
  T pivot = start[length / 2];
  T *lt;
  T *gt = end;
  for (lt = start; lt < gt; lt++) {
    if (less(*lt, pivot))
      continue;
    do {
      gt--;
      if (gt == lt)
        goto escape;
    } while (!less(*gt, pivot));
    std::swap(*lt, *gt);
  }

  escape:;
  if (start == lt) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (!less(pivot, *ptr))
        std::swap(*ptr, *lt++);
    }
  } else {
    quicksortSwapPlusShell(start, lt, less);
  }
  quicksortSwapPlusShell(lt, end, less);
}

}