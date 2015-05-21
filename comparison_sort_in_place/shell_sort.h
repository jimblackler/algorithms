// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

namespace comparisonSortInPlace {

template <typename T>
void insertionSortWithOffset(T *start, T *end, T offset) {
  for (T *fwd = start + offset; fwd < end; fwd++) {
    T value = *fwd;
    T *rev;
    for (rev = fwd; rev >= start + offset; rev -= offset) {
      T v = rev[-offset];
      if (v <= value)
        break;
      *rev = v;
    }
    *rev = value;
  }
}

template <typename T>
void shellSort(T *start, T *end) {
  auto length = end - start;
  T divide = 9;
  for (auto offset = length / divide; offset > 1; offset /= divide) {
    insertionSortWithOffset(start, end, (T) offset);
  }
  insertionSortWithOffset(start, end, 1);
}
}