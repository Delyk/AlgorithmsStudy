#include "euclid.h"
#include <cstdarg>
#include <utility>
using namespace euclid;

int euclid::euclid_classic(int a, int b) {
  // Вычитаем большее из меньшего до тех пор пока числа не станут равны
  while (b) {
    if (a < b) {
      std::swap(a, b);
    }
    a -= b;
  }
  return a;
}

int euclid::euclid_remainder(int a, int b) {
  if (a < b) {
    std::swap(a, b);
  }
  // Делим пока остаток от деления больше нуля
  while (b) {
    int tmp = a % b;
    a = b;
    b = tmp;
  }
  return a;
}

int euclid::euclid_recursive(int a, int b) {
  if (b == 0) {
    return a;
  }
  return euclid::euclid_recursive(b, a % b);
}

int euclid::euclid_elipsis(int n, ...) {
  va_list args;
  va_start(args, n);
  if (!n) {
    return 0;
  }

  int result = va_arg(args, int);
  // Ищем НОД по два числа, пока не закончатся все числа
  for (int i = 1; i < n; i++) {
    int next = va_arg(args, int);
    result = euclid_remainder(result, next);
  }

  va_end(args);
  return result;
}

std::pair<int, int> euclid::euclid_Besu(int a, int b) {

  int x0 = 1, y0 = 0; // коэффициенты для a
  int x1 = 0, y1 = 1; // коэффициенты для b

  while (b) {
    int q = a / b;
    int r = a % b;

    int x2 = x0 - q * x1;
    int y2 = y0 - q * y1;

    a = b;
    b = r;

    x0 = x1;
    y0 = y1;
    x1 = x2;
    y1 = y2;
  }

  return {x0, y0};
}
