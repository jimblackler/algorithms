#pragma once

#import <type_traits>
#import <algorithm>

template<typename T, typename Predicate>
static T *median5(Predicate less, T *x) {
  T *a = x;
  T *b = x + 1;
  T *c = x + 2;
  T *d = x + 3;
  T *e = x + 4;

  if (less(*b, *a))
    std::swap(a, b);

  if (less(*d, *c))
    std::swap(c, d);

  if (less(*c, *a)) {
    std::swap(b, d);
    c = a;
  }

  if (less(*b, *e))
    std::swap(e, b);

  if (less(*e, *c)) {
    if (less(*b, *c))
      return b;
    return c;
  }

  if (less(*d, *e))
    return d;
  return e;
}

