#pragma once

template<typename T, typename Predicate>
static T *median3(Predicate less, T* x) {
  if (less(x[0], x[1])) {
    // 012 or 021 or 201
    if (less(x[1], x[2]))
      return x + 1; // 012
    // 021 or 201
    if (less(x[0], x[2]))
      return x + 2; // 021
    return x; // 201
  }
  // 102 or 120 or 210
  if (less(x[2], x[1]))
    return x + 1; // 210
  // 102 or 120
  if (less(x[0], x[2]))
    return x; // 102
  return x + 2; // 10x
}