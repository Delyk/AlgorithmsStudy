#pragma once
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

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
  // Перемещаем pivot в конец для удобства
  std::iter_swap(pivot, last - 1);
  auto index = first;

  // Перемещаем элементы меньше pivot в начало
  for (auto it = first; it < last - 1; it++) {
    if (comp(*it, value)) {
      std::iter_swap(it, index);
      index++;
    }
  }

  // Элементы больше или равные опорному автоматически будут в конце
  // Возвращаем pivot
  std::iter_swap(index, last - 1);
  return index;
}

template <typename RandomIt,
          typename Compare =
              std::less<typename std::iterator_traits<RandomIt>::value_type>>
void QuickSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
  // Базовый случай один или два элемента
  if (last - first <= 1)
    return;
  // Раздеяем элементы, возвращаем опорный
  auto pivot = Partition(first, last, comp);
  // Сортируем разделённые подмассивы
  QuickSort(first, pivot, comp);
  QuickSort(pivot + 1, last, comp);
}

/*
 * Сортировка слиянием
 */
template <typename RandomIt,
          typename Compare =
              std::less<typename std::iterator_traits<RandomIt>::value_type>>
static void Merge(RandomIt first, RandomIt mid, RandomIt last,
                  Compare comp = Compare()) {
  if (first == mid || last == mid) {
    return;
  }
  using ValueType = typename std::iterator_traits<RandomIt>::value_type;
  std::vector<ValueType> tmp;

  RandomIt left = first;
  RandomIt right = mid;
  while (left != mid && right != last) { // Проходим по половинам массива
    if (comp(*left, *right)) { // Добавляем в массив меньший из двух элементов
      tmp.push_back(*left);
      left++;
    } else {
      tmp.push_back(*right);
      right++;
    }
  }

  // Копируем оставшиеся элементы
  while (left != mid) {
    tmp.push_back(*left);
    left++;
  }
  while (right != last) {
    tmp.push_back(*right);
    right++;
  }
  // Перемещаем полученный массив в исходную последовательность
  std::move(tmp.begin(), tmp.end(), first);
}

template <typename RandomIt,
          typename Compare =
              std::less<typename std::iterator_traits<RandomIt>::value_type>>
void MergeSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
  if (last - first <= 1) {
    return;
  }
  RandomIt mid = first + (last - first) / 2;

  MergeSort(first, mid, comp);
  MergeSort(mid, last, comp);

  Merge(first, mid, last, comp);
}

} // namespace sort
