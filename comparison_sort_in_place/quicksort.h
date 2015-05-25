// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

namespace comparisonSortInPlace {

template <typename T, typename F>
void quicksort(T *start, T *end, F less) {
  auto length = end - start;
  if (length <= 1)
    return;
  T pivot = start[length / 2];
  T *divider = start;
  for (T *ptr = start; ptr < end; ptr++) {
    if (less(*ptr, pivot))
      std::swap(*ptr, *divider++);
  }

  if (start == divider) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (!less(pivot, *ptr))
        std::swap(*ptr, *divider++);
    }
  } else {
    quicksort(start, divider, less);
  }

  quicksort(divider, end, less);
}
}