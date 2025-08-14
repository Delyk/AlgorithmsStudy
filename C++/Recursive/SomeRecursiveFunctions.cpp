#include "SomeRecursiveFunctions.h"
#include <climits>
#include <cstdarg>
#include <cstddef>
#include <stdexcept>
#include <vector>

int recursive::sum(int size, ...) {
  va_list list; //Список для итерации по аргументам
  va_start(list, size);
  int result = 0;
  for (int i = 0; i < size; ++i) {
    result +=
        va_arg(list, int); //Получить следующий аргумент и передвинуть указатель
  }
  va_end(list); //Завершить элипсис
  return result;
}

int recursive::count(int size, ...) {
  // va_list args;
  // va_start(args, size);
  // va_end(args);
  return size;
}

int recursive::max(int count, ...) {
  va_list list; //Список для итерации по аргументам
  va_start(list, count);
  if (!count) {
    throw std::invalid_argument("Empty");
  }
  int max = INT_MIN;
  for (int i = 0; i < count; ++i) {
    int current = va_arg(list, int);
    if (max < current) {
      max = current;
    }
  }
  va_end(list); //Завершить элипсис
  return max;
}

int recursive::min(int count, ...) {
  va_list list; //Список для итерации по аргументам
  va_start(list, count);
  if (!count) {
    throw std::invalid_argument("Empty");
  }
  int min = INT_MAX;
  for (int i = 0; i < count; ++i) {
    int current = va_arg(list, int);
    if (min > current) {
      min = current;
    }
  }
  va_end(list); //Завершить элипсис
  return min;
}

//Рекурсивная сумма
int recursive::sum(const std::vector<int> &array, std::size_t index) {
  if (array.empty()) {
    return 0;
  }
  if (index == array.size() - 1) {
    return array[index]; //Базовый случай
  }
  return array[index] + sum(array, index + 1); //Рекурсия
}

//Рекурсивный подсчёт кол-ва элементов в массиве
int recursive::count(const std::vector<int> &array, std::size_t index) {
  if (array.empty()) {
    return 0;
  }
  if (index == array.size() - 1) {
    return 1;
  }
  return 1 + count(array, index + 1);
}

//Поиск максимального
int recursive::max(const std::vector<int> &array, std::size_t index) {
  if (array.empty()) {
    throw std::invalid_argument("Empty array");
  }
  if (index == array.size() - 1) {
    return array[index];
  }
  int current_max = max(array, index + 1);
  return array[index] > current_max ? array[index] : current_max;
}

//Поиск минимального
int recursive::min(const std::vector<int> &array, std::size_t index) {
  if (array.empty()) {
    throw std::invalid_argument("Empty array");
  }
  if (index == array.size() - 1) {
    return array[index];
  }
  int current_min = min(array, index + 1);
  return array[index] < current_min ? array[index] : current_min;
}
