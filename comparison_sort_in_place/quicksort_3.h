// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#import <type_traits>
#import <algorithm>
#import "median_3.h"

namespace comparisonSortInPlace {

template<typename T, typename Predicate>
void quicksort3(T start, T end, Predicate less) {
  auto length = end - start;
  if (length <= 1) {
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

  T a = start;  // [start, a) is <pivot region
  T b = start;  // [b, ptr) is >pivot region
  for (T ptr = start; ptr < pivot; ptr++) {
    if (less(*pivot, *ptr))
      continue;

    if (less(*ptr, *pivot)) {
      if (a == b) {
        a++;
      } else {
        std::swap(*ptr, *a++);
      }
    }
    std::swap(*ptr, *b++); // Extend=pivot region

  }
  std::swap(*b++, *pivot);
  quicksort3(start, a, less);  // Sort <pivot region
  quicksort3(b, end, less);  // Sort >pivot region
}
}