// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

namespace comparisonSortInPlace {

template <typename T, typename Predicate, typename Size, typename Method>
void quicksort(T *start, T *end, Predicate less, Size minSize, Method nextMethod) {
  auto length = end - start;
  if (length <= minSize) {
    nextMethod(start, end);
    return;
  }
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
    quicksort(start, divider, less, minSize, nextMethod);
  }

  quicksort(divider, end, less, minSize, nextMethod);
}
}