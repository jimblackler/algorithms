// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>

namespace comparisonSortInPlace {

template <typename T, typename Predicate>
void quicksortSwap(T *start, T *end, Predicate less) {
  auto length = end - start;
  if (length <= 2) {
    if (length == 2 && less(start[1], *start))
      std::swap(*start, start[1]);
    return;
  }
  T *lt;
  T *pivot = end - 1;
  std::swap(start[length / 2], *pivot);
  T *ge = pivot;
  for (lt = start; lt < ge; lt++) {
    if (less(*lt, *pivot))
      continue;
    do {
      ge--;
      if (ge == lt)
        goto escape;
    } while (!less(*ge, *pivot));
    std::swap(*lt, *ge);
  }

  escape:;
  if (start == lt) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (!less(*pivot, *ptr)) {
        std::swap(*ptr, *lt++);
      }
    }
  } else {
    quicksortSwap(start, lt, less);
    std::swap(*pivot, *lt++);
  }
  quicksortSwap(lt, end, less);
}

};