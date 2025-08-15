#pragma once
#include <algorithm>
#include <iterator>
#include <map>

namespace sort {

/* Сортировка выбором
 */
template <typename BidirectIt,
          typename Compare =
              std::less<typename std::iterator_traits<BidirectIt>::value_type>>
void SelectionSort(BidirectIt first, BidirectIt last,
                   Compare comp = Compare()) {
  if (first == last)
    return;
  for (BidirectIt it = first; it != last;
       it++) { // Внешний цикл отделяет сортированную часть от несортированной
    BidirectIt min = it; // За минимальный берём первый несортированный
    for (BidirectIt cursor = std::next(it); cursor != last;
         cursor++) { // Внутренний цикл ищет минимальный элемент в
                     // несортированной части
      if (comp(*cursor, *min)) {
        min = cursor;
      }
    }
    if (min != it) {
      std::iter_swap(it, min); // Меняем местами первый несортированный элемент
                               // и найденный минимальный
    }
  }
}

/*
 * Быстрая сортировка
 */

template <typename RandomIt,
          typename Compare =
              std::less<typename std::iterator_traits<RandomIt>::value_type>>
static RandomIt Partition(RandomIt first, RandomIt last,
                          Compare comp = Compare()) {
  auto pivot = first + (last - first) / 2;
  auto value = *pivot;
  //Перемещаем pivot в конец для удобства
  std::iter_swap(pivot, last - 1);
  auto index = first;

  //Перемещаем элементы меньше pivot в начало
  for (auto it = first; it < last - 1; it++) {
    if (comp(*it, value)) {
      std::iter_swap(it, index);
      index++;
    }
  }

  //Элементы больше или равные опорному автоматически будут в конце
  //Возвращаем pivot
  std::iter_swap(index, last - 1);
  return index;
}

template <typename RandomIt,
          typename Compare =
              std::less<typename std::iterator_traits<RandomIt>::value_type>>
void QuickSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
  //Базовый случай один или два элемента
  if (last - first <= 1)
    return;
  //Раздеяем элементы, возвращаем опорный
  auto pivot = Partition(first, last, comp);
  //Сортируем разделённые подмассивы
  QuickSort(first, pivot, comp);
  QuickSort(pivot + 1, last, comp);
}
} // namespace sort
