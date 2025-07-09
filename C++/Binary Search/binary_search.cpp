#include "binary_search.h"
#include <algorithm>
#include <vector>

int binary_search(const std::vector<int> &array, int item) {

  int min = 0;
  int max = array.size() - 1;

  while (min <= max) {
    int mid = min + (max - min) / 2; // Выбираем средний элемент массива
    int guess = array[mid];
    if (guess == item) { // Если выбранный элемент и есть нужный, то возвращаем
                         // его индекс
      return mid;
    } else if (guess > item) { // Если элемент больше, то ищем в меньшей
                               // половине
      max =
          mid - 1; // Отсекаем индексы всех элементов больше или равные среднему
    } else if (guess < item) { // Если элемент меньше, то ищем в большей
                               // половине
      min =
          mid + 1; // Отсекаем индексы всех элементов меньше или равные среднему
    }
  }

  return -1;
}

// Рекурсивная реализация бинарного поиска
static int binary_search_recursive_realization(const std::vector<int> &array,
                                               int min, int max, int item) {
  if (min <= max) {
    int mid = min + (max - min) / 2;
    int guess = array[mid];

    if (guess == item) {
      return mid;
    } else if (guess > item) {
      return binary_search_recursive_realization(array, min, mid - 1, item);
    } else if (guess < item) {
      return binary_search_recursive_realization(array, mid + 1, max, item);
    }
  }
  return -1;
}

int binary_search_recursive(const std::vector<int> &array, int item) {
  return binary_search_recursive_realization(array, 0, array.size() - 1, item);
}

static int binary_search_left(const std::vector<int> &array, int item) {
  int min = 0;
  int max = array.size() - 1;
  int result = -1;

  while (min <= max) {
    int mid = min + (max - min) / 2;
    int guess = array[mid];

    if (guess > item) {
      max = mid - 1;
    } else if (guess < item) {
      min = mid + 1;
    } else if (guess == item) {
      result = mid;
      max = mid - 1; // Чтобы найти крайнее левое вхождение, после нахождения
                     // числа продолжаем поиск в левой части
    }
  }

  return result;
}

static int binary_search_right(const std::vector<int> &array, int item) {
  int min = 0;
  int max = array.size() - 1;
  int result = -1;

  while (min <= max) {
    int mid = min + (max - min) / 2;
    int guess = array[mid];

    if (guess > item) {
      max = mid - 1;
    } else if (guess < item) {
      min = mid + 1;
    } else if (guess == item) {
      result = mid;
      min = mid + 1; // Чтобы найти крайнее правое вхождение, после нахождения
                     // числа продолжаем поиск в правой части
    }
  }

  return result;
}

int binary_search_count(const std::vector<int> &array, int item) {
  // Чтобы подсчитать количество вхождений числа в массив находим крайнее левое
  // и крайнее правое вхождение числа в массив. Отнимаем правый индекс от левого
  int left = binary_search_left(array, item);
  int right = binary_search_right(array, item);

  if ((left == -1) && (right == -1))
    return -1;
  return right - left + 1;
}
