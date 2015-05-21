// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

namespace comparisonSortInPlace {

template <typename T>
void quicksort(T *start, T *end) {
  auto length = end - start;
  if (length <= 1)
    return;
  T pivot = start[length / 2];
  T *divider = start;
  for (T *ptr = start; ptr < end; ptr++) {
    T v = *ptr;
    if (v >= pivot)
      continue;
    *ptr = *divider;
    *divider++ = v;
  }

  if (start == divider) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (*ptr != pivot)
        continue;
      *ptr = *divider;
      *divider++ = pivot;
    }
  } else {
    quicksort(start, divider);
  }

  quicksort(divider, end);
}
}