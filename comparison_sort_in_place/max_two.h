// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#import <type_traits>
#import <algorithm>

namespace comparisonSortInPlace {

template <typename T, typename Predicate>
void max_two(T *start, T *end, Predicate less) {
  auto length = end - start;
  if (length == 2 && less(start[1], start[0]))
    std::swap(start[0], start[1]);
}
}