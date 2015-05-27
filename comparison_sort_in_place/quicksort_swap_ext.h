// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>

namespace comparisonSortInPlace {

template <typename T, typename Predicate, typename Size, typename Method>
void quicksortSwapExt(T *start, T *end, Predicate less, Size minSize, Method nextMethod) {
  auto length = end - start;
  if (length <= minSize) {
    nextMethod(start, end);
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
    quicksortSwapExt(start, lt, less, minSize, nextMethod);
    std::swap(*pivot, *lt++);
  }
  quicksortSwapExt(lt, end, less, minSize, nextMethod);
}


};