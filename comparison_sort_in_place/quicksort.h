// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#import <type_traits>
#import "median_3.h"
#import "median_5.h"

namespace comparisonSortInPlace {

template<typename T, typename Predicate, typename Size, typename Method>
void quicksort(T start, T end, Predicate less, Size minSize, Method nextMethod) {
  auto length = end - start;
  if (length <= minSize) {
    nextMethod(start, end);
    return;
  }
  T pivot = end - 1;
  T c = start + length / 2;
  if (length < 40)
    std::swap(*median3(less, c - 1), *pivot);
  else
    std::swap(*median5(less, c - 2), *pivot);
  T divider = start;
  for (T ptr = start; ptr < pivot; ptr++) {
    if (less(*ptr, *pivot))
      std::swap(*ptr, *divider++);
  }

  if (start == divider) {
    for (T ptr = divider; ptr < pivot; ptr++) {
      if (!less(*pivot, *ptr))
        std::swap(*ptr, *divider++);
    }
  } else {
    quicksort(start, divider, less, minSize, nextMethod);
  }
  std::swap(*pivot, *divider++);
  quicksort(divider, end, less, minSize, nextMethod);
}
}