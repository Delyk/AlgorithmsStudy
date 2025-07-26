#include "sort.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <chrono>
#include <functional>
#include <gtest/gtest.h>
#include <iomanip>
#include <ios>
#include <ratio>
#include <vector>
#define TEMPLATE_PARAMS                                                        \
  typename BidirectIt,                                                         \
      typename Compare =                                                       \
          std::less<typename std::iterator_traits<BidirectIt>::value_type>
#define SORT_PARAMS BidirectIt first, BidirectIt last, Compare comp = Compare()
using namespace sort;

// Обёртки алгоритмов сортировки
struct SelectionSortWrapper {
  template <TEMPLATE_PARAMS> void operator()(SORT_PARAMS) const {
    SelectionSort(first, last, comp);
  }
};

// Список алгоритмов сортировки
using SortAlgorithms = ::testing::Types<SelectionSortWrapper>;

// Функция проверки сортировки
template <typename Sorter, typename Container, typename Compare = std::less<>>
void TestSort(const Sorter &sorter, const Container &input,
              Compare comp = Compare()) {
  size_t count = input.size();
  Container control = input;
  Container data = control;

  auto start = std::chrono::high_resolution_clock::now();

  sorter(data.begin(), data.end(), comp);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed_ms = end - start;

  EXPECT_TRUE(std::is_sorted(data.begin(), data.end(), comp));
  std::sort(control.begin(), control.end(), comp);
  EXPECT_EQ(control, data);

  std::cout << std::fixed << "Sorted: " << count
            << " Time: " << elapsed_ms.count() << " ms\n";
  testing::Test::RecordProperty("elements_sorted", static_cast<int>(count));
  testing::Test::RecordProperty("duration_ms", elapsed_ms.count());
}

// Шаблонный класс для каждого алгоритма
template <typename Sorter> class SortTest : public ::testing::Test {
public:
  Sorter sorter;

  static std::vector<std::vector<int>> datasetsInt() {
    return {
        {},
        {1},                           // один элемент
        {1, 2, 3, 4, 5},               // уже отсортирован
        {5, 4, 3, 2, 1},               // обратный порядок
        {3, 5, 1, 2, 4, 6},            // случайные данные
        {3, 5, 3, 1, 2, 5},            // с дубликатами
        {9, 7, 5, 3, 1, 2, 4, 6, 8, 0} // больше элементов
    };
  }
};

TYPED_TEST_SUITE(SortTest, SortAlgorithms);

TYPED_TEST(SortTest, SortAscending) {
  for (const auto &data : SortTest<TypeParam>::datasetsInt()) {
    TestSort(this->sorter, data, std::less<int>{});
  }
}

TYPED_TEST(SortTest, SortDescending) {
  for (const auto &data : SortTest<TypeParam>::datasetsInt()) {
    TestSort(this->sorter, data, std::greater<int>{});
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
