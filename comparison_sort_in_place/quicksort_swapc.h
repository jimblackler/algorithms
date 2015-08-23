// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>

namespace comparisonSortInPlace {

template<typename T, typename Predicate>
void quicksortSwapb(T start, T end, Predicate less) {
  auto length = end - start;
  if (length <= 1)
    return;


  T useStart;
  T pivot;
  T c = start + length / 2;
  auto r = 9;
  if (length <= r * 2) {
    pivot = end - 1;
    std::swap(*c, *pivot);
    useStart = start;
  } else {
    auto rs = r / 2;
    auto rl = r - rs;
    auto b0 = c - rs;
    auto b1 = c + rl;
    insertionSort(b0, b1, less);
    useStart = start + rs;
    pivot = end - rl;
    std::swap_ranges(b0, c, start);
    std::swap_ranges(c, b1, pivot);
  }

  T ge = pivot;

  for (T lt = useStart; lt < ge; lt++) {
    if (!less(*lt, *pivot)) {
      while (--ge != lt) {
        if (less(*ge, *pivot)) {
          std::swap(*lt, *ge);
          break;
        }
      }
    }
  }

  if (useStart == ge) {
    for (T ptr = ge; ptr < pivot; ptr++) {
      if (!less(*pivot, *ptr))
        std::swap(*ptr, *ge++);
    }
  } else {
    quicksortSwapb(start, ge, less);
  }
  std::swap(*pivot, *ge++);
  quicksortSwapb(ge, end, less);
}

};