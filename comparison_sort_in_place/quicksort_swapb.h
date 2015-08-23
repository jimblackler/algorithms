// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>

#include "median_3.h"
#include "median_5.h"
#include "median_by_partial_sort.h"

namespace comparisonSortInPlace {

template<typename T, typename Predicate>
void quicksortSwap(T start, T end, Predicate less) {
  auto length = end - start;
  if (length <= 1)
    return;

  T pivot = end - 1;
  T c = start + length / 2;
  if (length < 14)
    std::swap(*c, *pivot);
  else if (length < 79)
    std::swap(*median3(less, c - 1), *pivot);
  else if (length < 227)
    std::swap(*median5(less, c - 2), *pivot);
  else if (length < 415)
    std::swap(*medianByPartialSort(c - 3, c + 4, less), *pivot);
  else
    std::swap(*medianByPartialSort(c - 4, c + 5, less), *pivot);

  T ge = pivot;

  for (T lt = start; lt < ge; lt++) {
    if (!less(*lt, *pivot)) {
      while (--ge != lt) {
        if (less(*ge, *pivot)) {
          std::swap(*lt, *ge);
          break;
        }
      }
    }
  }

  if (start == ge) {
    for (T ptr = ge; ptr < pivot; ptr++) {
      if (!less(*pivot, *ptr))
        std::swap(*ptr, *ge++);
    }
  } else {
    quicksortSwap(start, ge, less);
  }
  std::swap(*pivot, *ge++);
  quicksortSwap(ge, end, less);
}

};