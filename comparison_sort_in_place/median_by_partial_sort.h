// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>
#include <algorithm>

namespace comparisonSortInPlace {

template<typename T, typename F>
T medianByPartialSort(T start, T end, F less) {
  auto medianPosition = start + (end - start) / 2;
  for (T fwd = start + 1; fwd < end; fwd++) {
    auto remain = end - fwd;

    for (T rev = fwd; rev > start && rev > medianPosition - remain; rev--) {
      if (less(*rev, rev[-1]))
        std::swap(*rev, rev[-1]);
      else
        break;
    }
  }
  return medianPosition;
}

}