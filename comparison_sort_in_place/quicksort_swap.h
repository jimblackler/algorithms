// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>

#include "median_3.h"
#include "median_5.h"

namespace comparisonSortInPlace {

template<typename T, typename Predicate, typename Size, typename Method>
void quicksortSwap(T start, T end, Predicate less, Size minSize, Method nextMethod) {
  auto length = end - start;
  if (length <= minSize) {
    nextMethod(start, end);
    return;
  }

  T pivot = end - 1;
  T c = start + length / 2;
  if (length < 20)
    std::swap(*c, *pivot);
  else if (length < 40)
    std::swap(*median3(less, c - 1), *pivot);
  else
    std::swap(*median5(less, c - 2), *pivot);

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
    quicksortSwap(start, ge, less, minSize, nextMethod);
  }
  std::swap(*pivot, *ge++);
  quicksortSwap(ge, end, less, minSize, nextMethod);
}

};