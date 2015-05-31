// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <thread>
#include "quicksort_swap.h"

namespace comparisonSortInPlace {

template<typename T, typename Predicate, typename Size, typename Method>
void quicksortSwapThreaded(T start, T end, Predicate less, Size minSize,
    Size minSize2, Method nextMethod) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwap(start, end, less, minSize2, nextMethod);

  T lt;
  T pivot = end - 1;
  std::swap(start[length / 2], *pivot);
  T ge = pivot;
  for (lt = start; lt < ge; lt++) {
    if (less(*lt, *pivot))
      continue;
    do {
      ge--;
      if (ge == lt)
        goto escape;
    } while (!less(*ge, *pivot));
    std::swap(*lt, *ge);
  }

  escape:;
  if (start == lt) {
    for (T ptr = start; ptr < end; ptr++) {
      if (!less(*pivot, *ptr)) {
        std::swap(*ptr, *lt++);
      }
    }
    quicksortSwapThreaded(lt, end, less, minSize, minSize2, nextMethod);
  } else {

    std::thread t1([=]() {
        quicksortSwapThreaded(start, lt, less, minSize, minSize2, nextMethod);
    });
    std::swap(*pivot, *lt++);
    quicksortSwapThreaded(lt, end, less, minSize, minSize2, nextMethod);
    t1.join();
  }
}

}