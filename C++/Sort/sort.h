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

} // namespace sort
